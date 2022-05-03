#include <ESP32Tar.h>
#include <TestMQTTClient.h>

ESP32Tar *pTar = nullptr;

ESP32Tar::ESP32Tar(void)
{
    pTar = this;
    Init();
}

void ESP32Tar::Init(void)
{
    Serial2.begin(9600);
    xTaskCreate(handle, "", 5000, this, 1, nullptr);
}

void ESP32Tar::handle(void *pParam)
{
    /*分析m_readList中的数据*/
    ESP32Tar &espTar = *(ESP32Tar *)pParam;
    while (1)
    {
        while (Serial2.available())
        {
            espTar.AnalysisReadList(Serial2.read());
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); //休息1秒
    }
}

int ESP32Tar::availableForWrite(void)
{
    return Serial2.availableForWrite();
}

void ESP32Tar::flush(void)
{
    Serial2.flush();
}

void ESP32Tar::write(uint8_t data)
{
    Serial2.write(data);
}

/**
 * @brief 串口收到数据后会被调用
 *
 * @param data
 */
void ESP32Tar::OnReceive(CircularQueue<uint8_t> &data)
{
    switch (data.pop_front())
    {
    case 1: //接收温度
    {
        uint16_t payload;
        uint8_t *pPayloadBytes = (uint8_t *)(&payload);
        pPayloadBytes[0] = data.pop_front();
        pPayloadBytes[1] = data.pop_front();
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
        pMqttClient->PublishFrom_2th_SubTopic("temperature", (uint8_t *)&temperature, 8);
    }
    }
}
