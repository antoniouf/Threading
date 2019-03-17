// CThreadSafeQueue.cpp
#include <ThreadSafeQueue.h>

template <typename T>
inline CThreadSafeQueue<T>::CThreadSafeQueue(const CThreadSafeQueue &Source)
{
    std::lock_guard<std::mutex> Lock(Source.m_Mtx);
    m_data = Source.m_data;
}

template <typename T>
void CThreadSafeQueue<T>::push(T Item)
{
    std::lock_guard<std::mutex> Lock(m_Mtx);
    m_data.push(Item);
    m_cond_var.notify_one();
}

template<typename T>
bool CThreadSafeQueue<T>::try_pop(T & Val)
{
    std::lock_guard<std::mutex> Lock(m_Mtx);
    if (m_data.empty())
    {
        return false;
    }
    Val = m_data.front();
    m_data.pop();

    return true;
}

template<typename T>
std::shared_ptr<T> CThreadSafeQueue<T>::try_pop()
{
    std::lock_guard<std::mutex> Lock(m_Mtx);
    if (m_data.empty())
    {
        return false;
    }
    auto psFront = std::make_shared<T>(m_data.front());
    m_data.pop();

    return true;
}

template<typename T>
void CThreadSafeQueue<T>::wait_and_pop(T& Val)
{
    std::unique_lock<std::mutex> Lock(m_Mtx);
    m_cond_var.wait(Lock, [this] {return !m_data.empty(); });
    Val = m_data.front();
    m_data.pop();
}

template<typename T>
std::shared_ptr<T> CThreadSafeQueue<T>::wait_and_pop()
{
    std::unique_lock<std::mutex> Lock(m_Mtx);
    m_cond_var.wait(Lock, [this] {return !m_data.empty(); });
    auto psFront = std::make_shared<T>(m_data.front());
    m_data.pop();

    return psFront;
}

template<typename T>
bool CThreadSafeQueue<T>::empty() const
{
    std::lock_guard<std::mutex> Lock(m_Mtx);
    return m_data.empty();
}


