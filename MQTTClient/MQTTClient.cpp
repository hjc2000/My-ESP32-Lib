#include "MQTTClient.h"

//构造函数
MQTTClient::MQTTClient(void)
{
    /*先实例化wifi连接对象。不用担心对象实例化多次，类的内部有防止该行为的
    机制*/
    new ConnectWifi();
    //实例化成员对象
    m_pWifiClient = new WiFiClient();
    m_pPubSubClient = new PubSubClient(*m_pWifiClient);

    //设置MQTT服务器参数
    m_pPubSubClient->setServer(m_mqttServerIp, 1883);
    //设置收到消息后的回调函数
    auto fun = [this](char *topic, uint8_t *payload, unsigned int length) -> void
    {
        OnReceive(topic, payload, length);
    };
    m_pPubSubClient->setCallback(fun);
    //创建新线程，用来执行一些必须循环执行的任务
    xTaskCreate(LoopThread, "", 5000, this, 1, &m_hLoopThread);
}

//析构函数
MQTTClient::~MQTTClient(void)
{
    //删除顺序与构造顺序相反，先构造的后删除
    vTaskDelete(m_hLoopThread); //先把线程停止
    delete m_pPubSubClient;
    delete m_pWifiClient;
}

/**
 * @brief 将mqtt的接收回调函数的形参传递给该方法，可以得到一个格式化
 * 的字符串，格式为 【主题：xxxx 内容：xxxx】
 *
 * @param topic
 * @param payload
 * @param length
 * @return String
 */
String MQTTClient::FormateMqttContext(char *topic, uint8_t *payload, unsigned int length)
{
    String str = "主题：" + String(topic) + " 内容：";
    payload[length] = '\0';
    str = str + (char *)payload;
    return str;
}

//该方法用于连接到MQTT服务器
void MQTTClient::ConnectToMQTTServer(void)
{
    /*不能在wifi没有连接的情况下连接MQTT*/
    if (WiFi.status() == WL_CONNECTED)
    {
        /*用来提供MQTT的客户端ID，通过这种途径为该类被实例化多次，建立多个MQTT
        连接提供了可能*/
        auto getClientId = []() -> String
        {
            static uint16_t count = 0;
            return "esp32-" + WiFi.macAddress() + "-" + count++;
        };

        String clientId = getClientId();
        //尝试进行数次连接并返回结果
        auto tryConnect = [this, clientId]() -> bool
        {
            for (uint8_t i = 0; i < 5; i++)
            {
                if (m_pPubSubClient->connect(clientId.c_str()))
                {
                    return true;
                }
                vTaskDelay(pdMS_TO_TICKS(2000));
            }
            return false;
        };

        if (tryConnect())
        {
            Serial.println(clientId + " 连接成功");
            OnConnected(); //触发连接完成事件
        }
        else
        {
            Serial.println(clientId + " 连接失败");
            Serial.println("返回码：" + m_pPubSubClient->state());
        }
    }
}

/*该线程用来检查连接是否断开，断开则重连以及执行mqtt协议底层的一些
工作，例如接收和发送、保持心跳*/
void MQTTClient::LoopThread(void *pObj)
{
    MQTTClient *pThis = (MQTTClient *)pObj;
    while (1)
    {
        if (pThis->m_pPubSubClient->connected())
        {
            //如果当前状态为已连接
            /*注意，loop函数不只是用来保持心跳的，还用来接收和发送消息，
            所以loop被调用的时间间隔越小越好*/
            pThis->m_pPubSubClient->loop();
        }
        else
        {
            pThis->ConnectToMQTTServer();
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

//向MQTT服务器发布消息
void MQTTClient::Publish(String topic, String msg)
{
    m_pPubSubClient->publish(topic.c_str(), msg.c_str());
}

//向MQTT服务器发布消息
void MQTTClient::Publish(String topic, uint8_t *buff, uint32_t length)
{
    m_pPubSubClient->publish(topic.c_str(), buff, length);
}

//订阅主题
bool MQTTClient::Subscribe(String topic)
{
    return m_pPubSubClient->subscribe(topic.c_str());
}
