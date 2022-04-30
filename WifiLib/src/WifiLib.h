#ifndef WifiLib_h
#define WifiLib_h
#include <WiFi.h>
#include "Delegate.h"

/**
 * @brief 该类的对象只能通过 new 创建，否则离开作用域后就被销毁了
 *
 */
class ConnectWifi
{
public:
    ConnectWifi(void);
    static void connectThread(void *pObj);

private:
    // String m_ssid = "HUAWEI-5CHJTT_HiLink";
    // String m_password = "moter411";
    String m_ssid = "Hjc";
    String m_password = "123456789";
};

extern ConnectWifi *pWifi;

#endif
