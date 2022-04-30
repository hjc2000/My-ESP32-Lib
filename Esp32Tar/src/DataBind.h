#ifndef DataBind_h
#define DataBind_h
#include <map>
#include <string>
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
private:
    bool m_bNull = true;
    T m_value;

protected:
    virtual T get(void)
    {
        return m_value;
    }
    virtual void set(T value)
    {
        m_value = value;
    }

public:
    void operator=(T value)
    {
        m_bNull = false; //写过一次就不为空
        set(value);
    }
    T operator*(void)
    {
        return get();
    }
    operator bool()
    {
        return m_bNull;
    }
};

typedef IProperty<MqttMsg> IMqttProperty;
extern std::map<std::string, IProperty<MqttMsg> *> g_mqttDataMap;
#endif // DataBind_h