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
    if (length) //如果载荷长度大于0
    {
        String topicStr = String(topic);
        StringSplitter subTopics(topicStr, '/', 10);
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
        vector<uint8_t> data;
        data.push_back(1); //功能码
        data.push_back(1); //表示写LED
        data.push_back(payload[0]);
        };
}
