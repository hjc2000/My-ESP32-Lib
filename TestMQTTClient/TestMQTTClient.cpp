#include <TestMQTTClient.h>
#include <Esp32Tar.h>
#include <StringSplitter.h>

TestMQTTClient *pMqttClient = nullptr;

TestMQTTClient::TestMQTTClient(void)
{
    pMqttClient = this;
    InitCommandMap();
}

/**
 * @brief 连接成功后被调用
 */
void TestMQTTClient::OnConnected(void)
{
    //连接成功后订阅主题
    PublishFrom_3th_SubTopic("IsOnline", "true");
    Subscribe(GetClientId() + "/in/#");
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
    String topicStr = String(topic);
    StringSplitter subTopics(topicStr, '/', 10);
    if (subTopics.getItemCount() >= 3)
    {
        int index = 2; //第0个子主题为客户端ID，第1个子主题为"in"，所以从2开始
        auto it = m_commandMap.find(subTopics.getItemAtIndex(index++));
        if (it != m_commandMap.end()) //如果在map中找到处理该子主题的函数
        {
            (it->second)(payload, length);
        }
    }
}

void TestMQTTClient::InitCommandMap(void)
{
    m_commandMap["set_LED_state"] = [](uint8_t *payload, uint32_t length) -> void
    {
        if (length >= 1)
        {
            uint8_t data[2] = {1, payload[0]};
            pTar->sendData(1, data, sizeof(data));
        }
    };
    m_commandMap["read_LED_state"] = [](uint8_t *payload, uint32_t length) -> void
    {
        uint8_t readLedState = 0;
        pTar->sendData(1, &readLedState, 1);
    };
    m_commandMap["get_online_state"] = [this](uint8_t *payload, uint32_t length) -> void
    {
        Serial.println("收到获取在线状态的请求");
        PublishFrom_3th_SubTopic("IsOnline", "true");
    };
}
