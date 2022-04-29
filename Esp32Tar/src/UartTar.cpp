#include "UartTar.h"

void UartTar::DetectFrameHeader(void)
{
    if ((m_newData == 85) && (m_oldData == 85))
    {
        m_flag = 1;
    }
}

void UartTar::GetFrameLength(void)
{
    m_receiveCount = m_newData;
    //如果数据长度是85，会导致帧重启，所以使用0代替85，数据长度不可能
    //真的是0，因为至少会有1字节的功能码，即使后面不跟数据
    if (m_receiveCount == 0)
    {
        m_receiveCount = 85;
    }
}

bool UartTar::IsInsertedZero(void)
{
    if (m_newData == 0 && m_oldData == 85)
    {
        return true;
    }
    return false;
}

void UartTar::GetData(void)
{
    if (!IsInsertedZero())
    {
        //如果这个数据不是被插入的用来转义的0
        m_readBuff.push_back(m_newData);
        m_receiveCount--;
        if (m_receiveCount == 0)
        {
            /*已经收集到一个帧了*/
            OnReceive(m_readBuff);
            m_flag = 0; //回到等待帧头的状态
        }
    }
}

void UartTar::AnalysisReadList(uint8_t data)
{
    m_newData = data;
    DetectFrameHeader();
    switch (m_flag)
    {
    case 0: //等待帧头的到来
    {
        break;
    }
    case 1: //已经接收到帧头，此时应该重启帧
    {
        m_readBuff.clear();
        m_flag++;
        break;
    }
    case 2: //接收帧长度
    {
        GetFrameLength();
        m_flag++;
        break;
    }
    case 3: //接收数据
    {
        GetData();
        break;
    }
    }
    m_oldData = m_newData;
}

void UartTar::PushFrameHeader(void)
{
    if (m_sendCount == 85)
    {
        m_sendBuff.push_front(0);
    }
    else
    {
        m_sendBuff.push_front(m_sendCount);
    }
    m_sendCount = 0;
    m_sendBuff.push_front(85);
    m_sendBuff.push_front(85);
}

void UartTar::PushData(uint8_t data)
{
    /*如果数据域中出现85*/
    if (data == 85)
    {
        m_sendBuff.push_back(85);
        m_sendBuff.push_back(0);
    }
    else
    {
        m_sendBuff.push_back(data);
    }
    m_sendCount++;
}

void UartTar::StartSend(void)
{
    auto it = m_sendBuff.begin();
    while (it != m_sendBuff.end())
    {
        if (!availableForWrite())
        {
            flush();
        }
        write(*it);
        it = m_sendBuff.erase(it);
    }
}

void UartTar::tar(uint8_t data, bool bIsEnd)
{
    if (bIsEnd)
    {
        PushFrameHeader(); //添加帧头
        StartSend();
    }
    else
    {
        PushData(data);
    }
}

void UartTar::sendData(const uint8_t *buff, uint16_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        tar(buff[i], false);
    }
    tar(0, true);
}
