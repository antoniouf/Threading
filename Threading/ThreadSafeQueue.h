#ifndef ThreadSafeQueue_25E036CF1FB84D9EA139BD446F9B15A4
#define ThreadSafeQueue_25E036CF1FB84D9EA139BD446F9B15A4

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

template <typename T> class CThreadSafeQueue {
public:
    CThreadSafeQueue() = default;
    CThreadSafeQueue(const CThreadSafeQueue &);
    CThreadSafeQueue &operator=(const CThreadSafeQueue &) = delete;

    void push(T Item);

    bool try_pop(T &Val); // assign top item to Val
    std::shared_ptr<T> try_pop();

    void wait_and_pop(T &Val);
    std::shared_ptr<T> wait_and_pop();

    bool empty() const;

private:
    mutable std::mutex m_Mtx;
    std::queue m_data;
    std::condition_variable m_cond_var;
};

#endif // ThreadSafeQueue_25E036CF1FB84D9EA139BD446F9B15A4
