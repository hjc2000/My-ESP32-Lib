#ifndef TAR_H
#define TAR_H

#include <stdint.h>
#include <list>
#include "CircularQueue.h"
#include <functional>
#include "Delegate.h"
using namespace std;

class Tar
{
protected:
    void AnalysisReadList(uint8_t data);

    /**
     * @brief 继承后重载该函数，根据硬件平台进行初始化，例如：设置硬件串口、
     * 创建一个线程，不断地将数据传给AnalysisReadList方法等。
     * 要求继承该虚函数后，子类在构造函数中调用该函数
     *
     */
    virtual void Init(void) = 0;

    /**
     * @brief 串口的发送缓冲区剩余可写空间
     *
     * @return int
     */
    virtual int availableForWrite(void) = 0;

    /**
     * @brief 等待，直到串口将发送缓冲区的数据全部发出去
     *
     */
    virtual void flush(void) = 0;

    /**
     * @brief 向串口发送缓冲区写一个字节，要求串口能将发送缓冲区的数据自己发出去
     *
     * @param data 要被写入的字节
     * @return int
     */
    virtual void write(uint8_t data) = 0;

    /**
     * @brief 接收到一帧的数据后被调用
     *
     * @param data
     */
    virtual void OnReceive(CircularQueue<uint8_t> &data) = 0;

private:
    //读缓冲区，分析完的数据放到这里
    CircularQueue<uint8_t> m_readBuff = CircularQueue<uint8_t>(100);

    uint8_t m_flag = 0; //用来标志工作阶段
    uint8_t m_oldData = 0;
    uint8_t m_newData = 0;
    uint8_t m_receiveCount = 0; //对接收到的数据域字节数计数（减计数，在收到长度时赋值）

    /// <summary>
    /// 检测帧头，通过控制m_flag来控制解包程序的走向
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    void DetectFrameHeader(void);

    /// <summary>
    /// 获取帧的长度
    /// </summary>
    /// <param name=""></param>
    void GetFrameLength(void);

    /// <summary>
    /// 当前收到的数据是不是用来转义的被插入的0
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    bool IsInsertedZero(void);

    /// <summary>
    /// 正式接收数据
    /// </summary>
    /// <param name=""></param>
    void GetData(void);

private:
    /*将数据打包后写入发送缓冲区*/
    void tar(uint8_t data, bool bIsEnd);
    list<uint8_t> m_sendBuff;
    uint8_t m_sendCount = 0;     //对数据域个数进行计数
    void PushFrameHeader(void);  //将发送数据的长度放入发送缓冲区
    void PushData(uint8_t data); //将数据放入发送缓冲区
    void StartSend(void);        //开始将发送缓冲区的数据发送出去

public:
    //向串口发送数据
    void sendData(const uint8_t *buff, uint16_t size);
};
#endif // TAR_H
