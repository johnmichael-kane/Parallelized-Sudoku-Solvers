#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>

template <typename T>
class ThreadSafeQueue
{
private:
    mutable std::mutex mtx;
    std::queue<T> queue;
    std::condition_variable data_cond;

public:
    ThreadSafeQueue() {}
    ThreadSafeQueue(const ThreadSafeQueue &other)
    {
        std::lock_guard<std::mutex> lock(other.mtx);
        queue = other.queue;
    }

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(std::move(new_value));
        data_cond.notify_one();
    }

    bool try_pop(T &value)
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (queue.empty())
        {
            return false;
        }
        value = std::move(queue.front());
        queue.pop();
        return true;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mtx);
        return queue.empty();
    }

    void wait_and_pop(T &value)
    {
        std::unique_lock<std::mutex> lock(mtx);
        data_cond.wait(lock, [this]
                       { return !queue.empty(); });
        value = std::move(queue.front());
        queue.pop();
    }    
};
