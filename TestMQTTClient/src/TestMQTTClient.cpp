#include "TestMQTTClient.h"
#include "Esp32Tar.h"

TestMQTTClient *pMqttClient = nullptr;

TestMQTTClient::TestMQTTClient(void)
{
    pMqttClient = this;
}

/**
 * @brief 连接成功后被调用
 */
void TestMQTTClient::OnConnected(void)
{
    //连接成功后订阅主题
    if (m_pPubSubClient->connected())
    {
        if (Subscribe("esp32/command/#"))
        {
            Serial.println("订阅成功");
        }
        else
        {
            Serial.println("订阅失败");
        }
    }
}

/**
 * @brief 将mqtt的主题log作为打印日志的地方
 *
 * @param str 要被打印的内容
 */
void TestMQTTClient::MqttLog(String str)
{
    Publish("log", str);
}

/**
 * @brief 收到订阅的主题的数据后被回调
 *
 * @param topic 主题。C风格的字符串
 *
 * @param payload 一个uint8_t数组的首地址。注意，如果内容是字符串，它的
 * 末尾并没有被添加空字符，需要手动添加
 *
 * @param length payload的长度
 */
void TestMQTTClient::OnReceive(char *topic, uint8_t *payload, unsigned int length)
{
    if (String(topic) == "esp32/command/msp")
    {
        pTar->sendData(payload, length);
    }
}
