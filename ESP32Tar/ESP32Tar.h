#ifndef ESP32Tar_h
#define ESP32Tar_h
#include <UartTar.h>

class ESP32Tar : public UartTar
{
public:
    ESP32Tar(void);
    void OnReceive(Queue<uint8_t> &data) override;
    static void handle(void *pParam);

public:
    void ReceiveTemperature(Queue<uint8_t> &data);
    void ReceiveLedState(Queue<uint8_t> &data);
};
extern ESP32Tar *pTar;
#endif // ESP32Tar_h