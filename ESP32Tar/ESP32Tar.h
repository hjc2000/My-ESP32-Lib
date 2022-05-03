#ifndef ESP32Tar_h
#define ESP32Tar_h
#include <UartTar.h>

class ESP32Tar : public UartTar
{
public:
    ESP32Tar(void);

protected:
    void Init(void) override;
    int availableForWrite(void) override;
    void flush(void) override;
    void write(uint8_t data) override;
    void OnReceive(CircularQueue<uint8_t> &data) override;

public:
    static void handle(void *pParam);
};
extern ESP32Tar *pTar;
#endif // ESP32Tar_h