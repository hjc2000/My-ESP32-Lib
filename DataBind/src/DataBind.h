#ifndef DataBind_h
#define DataBind_h
#include <map>
#include <Arduino.h>

struct MqttMsg
{
    String topic;
    uint8_t *payload;
    int length;
};

template <class T>
class IProperty
{
public:
    virtual T get(void) = 0;
    virtual void set(T value) = 0;
};

#endif // DataBind_h