#ifndef ESP32MQTTClient_h
#define ESP32MQTTClient_h
#include <Arduino.h>
#include <WiFi.h>
#include "PubSubClient.h"
#include <stdint.h>
#include "WifiLib.h"
using namespace std;

/**
 * @brief 1. 该类不可被直接调用，只能被继承，其中纯虚函数提供了接口，
 * 这些接口是事件钩子，会在特定的事件发生时被调用。
 * 2. 该类基于 FreeRTOS
 *
 */
class MQTTClient
{
public:
    MQTTClient(void);
    ~MQTTClient(void);
    void Publish(String topic, String msg);
    void Publish(String topic, uint8_t *buff, uint32_t length);
    void ConnectToMQTTServer(void);
    bool Subscribe(String topic);
    String FormateMqttContext(char *topic, uint8_t *payload, unsigned int length);

protected:
    virtual void OnConnected(void) = 0; //连接成功后被调用
    //收到订阅的主题的数据时被回调
    virtual void OnReceive(char *topic, uint8_t *payload, unsigned int length) = 0;

protected:
    WiFiClient *m_pWifiClient = nullptr;
    PubSubClient *m_pPubSubClient = nullptr;
    const char *m_mqttServerIp = "192.168.31.186";
    TaskHandle_t m_hLoopThread = nullptr;

private:
    static void LoopThread(void *pObj);
};

#endif // ESP32MQTTClient_h