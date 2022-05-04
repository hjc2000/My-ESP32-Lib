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
    if (m_pPubSubClient->connected())
    {
        if (Subscribe(GetClientId() + "/command/#"))
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
    int count = subTopics.getItemCount();
    if (count >= 3)
    {
        //至少要3级子主题
        int index = 0;
        if (subTopics.getItemAtIndex(index++) == GetClientId())
        {
            if (subTopics.getItemAtIndex(index++) == "command")
            {
                auto it = m_commandMap.find(subTopics.getItemAtIndex(index++));
                if (it != m_commandMap.end())
                {
                    (it->second)(payload, length);
                }
            }
        }
    }
}

void TestMQTTClient::InitCommandMap(void)
{
    m_commandMap["msp"] = [](uint8_t *payload, uint32_t length) -> void
    {
        pTar->sendData(1, payload, length);
    };
}
