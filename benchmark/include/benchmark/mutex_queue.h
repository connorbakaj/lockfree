#pragma once

#include <mutex>
#include <queue>

namespace benchmark
{
template <typename T>
class MutexQueue
{
private:
    std::queue<T> queue_{};
    mutable std::mutex mutex_;

public:
    auto try_push(const T& item) -> bool
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(item);
        return true;
    }

    auto try_pop(T& item) -> bool
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return false;
        }
        item = queue_.front();
        queue_.pop();
        return true;
    }
};
}  // namespace benchmark