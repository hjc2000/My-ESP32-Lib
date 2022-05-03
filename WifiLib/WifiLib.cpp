#include "WifiLib.h"

/**
 * @brief ConnectWifi类的对象只允许被实例化一次，不要实例化多次。类
 * 内部有机制，第二次实例化的对象不会接管wifi模块。该指针指向第一次实例
 * 化的对象
 *
 */
ConnectWifi *pWifi = nullptr;

ConnectWifi::ConnectWifi(void)
{
    static bool bFirst = true;
    /*先判断是不是第一次被初始化，改类只允许有一个实例化对象*/
    if (bFirst)
    {
        bFirst = false;
        //设置wifi模式为无线终端模式
        WiFi.mode(WIFI_STA);
        xTaskCreate(connectThread, "", 2000, this, 1, nullptr);
        pWifi = this;
    }
    else
    {
        delete this; //该类的对象只允许通过new创建
    }
}

/**
 * @brief 用来保持wifi处于连接状态
 *
 * @param pObj ConnectWifi 类的对象的指针
 */
void ConnectWifi::connectThread(void *pObj)
{
    ConnectWifi *pThis = (ConnectWifi *)pObj;
    while (1)
    {
        //如果wifi不处于连接状态，则尝试连接
        if (WiFi.status() != WL_CONNECTED)
        {
            // WiFi.begin("Tenda_082840", "123456789");
            WiFi.begin(pThis->m_ssid.c_str(), pThis->m_password.c_str());
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
