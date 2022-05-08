#include <ESP32Tar.h>
#include <TestMQTTClient.h>

ESP32Tar *pTar = nullptr;

ESP32Tar::ESP32Tar(void) : UartTar(&Serial2)
{
    pTar = this;
    Serial2.begin(9600);
    xTaskCreate(handle, "", 5000, this, 1, nullptr);
}

void ESP32Tar::handle(void *pParam)
{
    /*分析m_readList中的数据*/
    ESP32Tar &espTar = *(ESP32Tar *)pParam;
    while (1)
    {
        espTar.loop();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void ESP32Tar::ReceiveTemperature(Queue<uint8_t> &data)
{
    float temp;
    uint8_t *pTempBuff = (uint8_t *)&temp;
    for (int i = 0; i < sizeof(float); i++)
    {
        pTempBuff[i] = data.pop();
    }
    double dTemp = temp;
    Serial.print("收到温度：");
    Serial.println(temp);
    pMqttClient->PublishFrom_3th_SubTopic("temperature", (uint8_t *)&dTemp, sizeof(double));
}
void ESP32Tar::ReceiveLedState(Queue<uint8_t> &data)
{
    uint8_t state = data.pop();
    pMqttClient->PublishFrom_3th_SubTopic("LedState", &state, 1);
}

/**
 * @brief 串口收到数据后会被调用
 *
 * @param data
 */
void ESP32Tar::OnReceive(Queue<uint8_t> &data)
{
    switch (data.pop())
    {
    case 1: // LED
    {
        ReceiveLedState(data);
        break;
    }
    case 2: //接收温度
    {
        ReceiveTemperature(data);
        break;
    }
    }
}
