#ifndef ESP32TAR_H
#define ESP32TAR_H
#include "Tar.h"
#include <Arduino.h>

class Esp32Tar : public Tar
{
public:
    Esp32Tar(void);

protected:
    void Init(void) override;
    int availableForWrite(void) override;
    void flush(void) override;
    void write(uint8_t data) override;
    void OnReceive(CircularQueue<uint8_t> &data) override;

public:
    static void handle(void *pParam);
};
extern Esp32Tar *pTar;
#endif // ESP32TAR_H