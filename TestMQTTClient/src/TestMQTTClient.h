#ifndef MQTTClient_h
#define MQTTClient_h
#include "MQTTClient.h"
#include <map>

class TestMQTTClient : public MQTTClient
{
public:
    TestMQTTClient(void);

protected:
    void OnConnected(void) override;
    void OnReceive(char *topic, uint8_t *payload, unsigned int length) override;

private:
    std::map<String, std::function<void(uint8_t *payload, unsigned int length)>> m_commandMap;
    void InitCommandMap(void);
};

extern TestMQTTClient *pMqttClient;
#endif // MQTTClient_h