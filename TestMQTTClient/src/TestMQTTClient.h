#ifndef MQTTClient_h
#define MQTTClient_h
#include "MQTTClient.h"

class TestMQTTClient : public MQTTClient
{
public:
    TestMQTTClient(void);
    void MqttLog(String str);

protected:
    void OnConnected(void) override;
    void OnReceive(char *topic, uint8_t *payload, unsigned int length) override;
};

extern TestMQTTClient *pMqttClient;
#endif // MQTTClient_h