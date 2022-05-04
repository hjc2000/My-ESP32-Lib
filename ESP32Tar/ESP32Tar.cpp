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
        vTaskDelay(pdMS_TO_TICKS(1000)); //休息1秒
    }
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
    case 1: //接收温度
    {
        uint16_t payload;
        uint8_t *pPayloadBytes = (uint8_t *)(&payload);
        pPayloadBytes[0] = data.pop();
        pPayloadBytes[1] = data.pop();
        auto f_getDoubleTemperature = [](uint16_t payload) -> double
        {
            payload &= 0x07ff;
            return payload / 16.0;
        };
        double temperature;
        if ((int16_t)payload >= 0)
        {
            temperature = f_getDoubleTemperature(payload);
        }
        else
        {
            temperature = -f_getDoubleTemperature(payload);
        }
        Serial.print("收到温度：");
        Serial.println(temperature);
        pMqttClient->PublishFrom_2th_SubTopic("temperature", (uint8_t *)&temperature, 8);
    }
    case 2:
    {
    }
    }
}
