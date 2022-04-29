#ifndef TASK_H
#define TASK_H

#include <list>
#include "stdint.h"
#include <functional>
using namespace std;

/// <summary>
/// 委托类
/// </summary>
class Delegate
{
public:
    /// <summary>
    /// 添加一个委托
    /// </summary>
    /// <param name="fun">委托函数</param>
    /// <returns>委托的ID</returns>
    uint16_t add(function<void(void *pParam)> fun)
    {
        m_id++;
        TaskStruct task = {m_id, fun};
        taskList.push_back(task);
        return m_id;
    }

    /// <summary>
    /// 删除一个委托
    /// </summary>
    /// <param name="id">要被删除的委托的ID</param>
    void deleteDelegate(uint16_t id)
    {
        for (auto it = taskList.begin(); it != taskList.end(); it++)
        {
            if ((*it).id == id)
            {
                m_itNext = taskList.erase(it); //找到了就把它删除
                if (it == m_itNow)             //如果删除的是handle中正在使用的迭代器
                {
                    m_itNow = taskList.end(); //迭代器失效后把它赋值为end
                }
                break;
            }
        }
    }

    /// <summary>
    /// 重载括号运算符，使得委托类的对象能像函数那样调用
    /// </summary>
    /// <param name="pParam"></param>
    void operator()(void *pParam)
    {
        Invoke(pParam);
    }

private:
    uint16_t m_id = 0;
    struct TaskStruct
    {
        uint16_t id;
        function<void(void *pParam)> fun;
    };
    list<TaskStruct>::iterator m_itNext;
    list<TaskStruct>::iterator m_itNow;
    list<TaskStruct> taskList; //任务列表

private:
    /// <summary>
    /// 执行委托列表中的函数
    /// </summary>
    /// <param name="pParam">委托方添加委托时交代的待传参数</param>
    void Invoke(void *pParam)
    {
        m_itNow = taskList.begin();
        while (m_itNow != taskList.end())
        {
            (*m_itNow).fun(pParam);
            //执行完回调函数后，回调函数可能把自己从委托中删除
            if (m_itNow == taskList.end())
            {
                //迭代器失效了
                m_itNow = m_itNext;
            }
            else
            {
                //迭代器没有失效
                m_itNow++;
            }
        }
    }
};

#endif // TASK_H
