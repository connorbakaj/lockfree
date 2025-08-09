#pragma once

#include <mutex>
#include <queue>

namespace benchmark
{
template <typename T, size_t Capacity>
class CondVarQueue
{
private:
    std::queue<T> queue_{};
    mutable std::mutex mutex_;
    std::condition_variable condition_;

public:
    auto try_push(const T& item) -> bool
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (queue_.size() >= Capacity) {
            return false;
        }
        queue_.push(item);
        condition_.notify_one();
        return true;
    }

    auto try_pop(T& item) -> bool
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return false;
        }
        item = queue_.front();
        queue_.pop();
        return true;
    }
};
}  // namespace benchmark