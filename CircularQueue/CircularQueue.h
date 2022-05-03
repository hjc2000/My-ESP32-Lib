#ifndef CIRCULARQUEUE_H
#define CIRCULARQUEUE_H
#include <stdint.h>
using namespace std;

/// <summary>
/// 循环队列类
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T>
class CircularQueue
{
public:
    uint8_t m_maxSizeofQueue = 50; //最大队列长度
    /// <summary>
    /// 无参构造函数，最大队列长度为50
    /// </summary>
    /// <param name=""></param>
    CircularQueue(void)
    {
        m_queue = new T[m_maxSizeofQueue];
    }

    /// <summary>
    /// 构造函数，传递一个参数指定最大队列长度
    /// </summary>
    /// <param name="maxQueueSize">最大队列长度</param>
    CircularQueue(uint8_t maxQueueSize)
    {
        m_queue = new T[maxQueueSize];
        m_maxSizeofQueue = maxQueueSize;
    }

private:
    /// <summary>
    /// 用来索引 CircularQueue 中的元素。因为循环队列要注意索引达到边界后跳转到
    /// 另一端，索引定义了这个类来专门处理索引值的递增和达到边界后的跳转，这样就可
    /// 以节省精力
    /// </summary>
    class QueueIndex
    {
    public:
        QueueIndex(void);

        /// <summary>
        /// 在 CircularQueue 的一个函数中传入this指针,这样 QueueIndex 就能够
        /// 访问到 CircularQueue 的对象，并进行索引
        /// </summary>
        /// <param name="pQueue">循环队列类的指针</param>
        QueueIndex(CircularQueue<T> *pQueue);

    private:
        uint8_t m_index = 0;    //当前的索引值
        uint8_t m_maxIndex = 0; //最大索引
    public:
        uint8_t operator*();  //返回当前的索引值
        void operator++(int); //索引值递增

    private:
        CircularQueue<T> *m_pQueue = nullptr; //指向要访问的对象
    };
    friend class QueueIndex;                                         //声明为友元类，这样才能访问 CircularQueue 的私有成员
    QueueIndex m_head = QueueIndex(this), m_tail = QueueIndex(this); //队列的头部索引和尾部索引

public: //向用户提供的对循环队列操作的方法
    /// <summary>
    /// 向队列末尾添加一个元素
    /// </summary>
    /// <param name="data"></param>
    /// <returns>是否添加成功</returns>
    bool push_back(T data);

    T pop_front(void);

    /// <summary>
    /// 队列是否为空
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    bool empty(void);

    /// <summary>
    /// 返回队列中元素的个数
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    uint8_t size(void);

    /// <summary>
    /// 队列剩余空间
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    uint8_t available(void);

    /// <summary>
    /// 清空队列
    /// </summary>
    /// <param name=""></param>
    void clear(void);

    /// <summary>
    /// 让 CircularQueue 能够像数组那样用方括号索引。返回的是引用
    /// </summary>
    /// <param name="index">索引数字</param>
    /// <returns>被索引的队列元素的引用</returns>
    T &operator[](uint8_t index);

private:
    /// <summary>
    /// 队列数组的指针
    /// </summary>
    T *m_queue;

    /// <summary>
    /// 队列中元素的个数
    /// </summary>
    uint8_t m_numOfData = 0;
};

template <typename T>
bool CircularQueue<T>::push_back(T data)
{
    if (m_numOfData != m_maxSizeofQueue)
    {
        /*队列没满*/
        m_queue[*m_tail] = data;
        m_tail++;
        m_numOfData++;
        return true;
    }
    else
    {
        return false;
    }
}

template <typename T>
T CircularQueue<T>::pop_front(void)
{
    T data = m_queue[*m_head];
    if (m_numOfData)
    {
        /*如果有数据*/
        m_head++;
        m_numOfData--;
    }
    return data;
}

template <typename T>
bool CircularQueue<T>::empty(void)
{
    return !m_numOfData;
}

template <typename T>
uint8_t CircularQueue<T>::size(void)
{
    return uint8_t(m_numOfData);
}

template <typename T>
uint8_t CircularQueue<T>::available(void)
{
    return uint8_t(m_maxSizeofQueue - m_numOfData);
}

template <typename T>
void CircularQueue<T>::clear(void)
{
    m_numOfData = 0;
    m_head = QueueIndex(this);
    m_tail = QueueIndex(this);
}

template <typename T>
inline T &CircularQueue<T>::operator[](uint8_t index)
{
    if (index >= m_numOfData)
    {
        index = m_numOfData - 1;
    }
    index += *m_head;
    if (index >= m_maxSizeofQueue)
    {
        index = index - m_maxSizeofQueue;
    }
    return m_queue[index];
}

template <typename T>
CircularQueue<T>::QueueIndex::QueueIndex(CircularQueue<T> *pQueue)
{
    m_pQueue = pQueue;
    m_maxIndex = m_pQueue->m_maxSizeofQueue - 1;
}

template <typename T>
uint8_t CircularQueue<T>::QueueIndex::operator*()
{
    return uint8_t(m_index); //返回当前的索引值
}

template <typename T>
void CircularQueue<T>::QueueIndex::operator++(int)
{
    if (m_index < m_maxIndex)
    {
        m_index++; //小于最大索引就递增
    }
    else
    {
        m_index = 0; //等于最大索引就回零
    }
}

#endif // CIRCULARQUEUE_H
