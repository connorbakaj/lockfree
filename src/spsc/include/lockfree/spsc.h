#pragma once

#include <atomic>
#include <cassert>
#include <memory>
#include <type_traits>
#include <utility>

namespace lockfree::queue
{

// Cache line size for most modern CPUs
constexpr size_t CACHE_LINE_SIZE = 64;

// Align to cache line to prevent false sharing
#define CACHE_ALIGNED alignas(CACHE_LINE_SIZE)

template <size_t T>
concept NonZeroMultipleOfTwo = (T & (T - 1)) == 0 && T >= 2;

template <typename T>
concept EfficientValueSemantics = requires {
    { std::is_trivially_copyable_v<T> || std::is_move_constructible_v<T> };
};

/// @brief Ultra-high performance lock-free single-producer single-consumer (SPSC) queue
/// @remarks Optimized for minimal latency with:
/// - Memory ordering optimizations
/// - Cache-friendly design
/// - Compile-time size validation
/// - Minimal branching in hot paths
/// @tparam T Queue element value type
/// @tparam Capacity The maximum number of elements in the queue
template <typename T, size_t Capacity>
    requires EfficientValueSemantics<T> && NonZeroMultipleOfTwo<Capacity>
class Spsc
{
private:
    static constexpr size_t MASK = Capacity - 1;

    /// @brief Storage with proper alignment
    struct alignas(T) Slot {
        T data;
    };

    // Separate cache lines for producer and consumer to eliminate false sharing
    CACHE_ALIGNED std::atomic<size_t> head_{0};  // Consumer position
    CACHE_ALIGNED std::atomic<size_t> tail_{0};  // Producer position
    CACHE_ALIGNED Slot slots_[Capacity];

public:
    Spsc() = default;
    ~Spsc() = default;

    // Non-copyable, non-movable for safety
    Spsc(const Spsc&) = delete;
    auto operator=(const Spsc&) -> Spsc& = delete;
    Spsc(Spsc&&) = delete;
    auto operator=(Spsc&&) -> Spsc& = delete;

    /// @brief Producer operation - enqueue element
    /// Returns true if successful, false if queue is full
    /// Uses relaxed ordering for maximum performance
    /// @tparam U Type to push to the queue
    /// @param item The item to push to the queue
    /// @return true if item was pushed to the queue, false otherwise
    template <typename U>
    [[nodiscard]] auto constexpr try_push(U&& item) noexcept -> bool
    {
        const size_t current_tail = tail_.load(std::memory_order_relaxed);
        const size_t next_tail = (current_tail + 1) & MASK;

        // Check if queue is full using acquire ordering to synchronize with consumer
        if (next_tail == head_.load(std::memory_order_acquire)) [[unlikely]] {
            return false;
        }

        // Store the item
        if constexpr (std::is_trivially_copyable_v<T>) {
            slots_[current_tail & MASK].data = std::forward<U>(item);
        } else {
            std::uninitialized_fill_n(&slots_[current_tail & MASK].data, T(std::forward<U>(item)));
        }

        // Publish the item with release ordering
        tail_.store(next_tail, std::memory_order_release);
        return true;
    }

    /// @brief Dequeue element
    /// Uses relaxed ordering for maximum performance
    /// @param item The will be filled with value from queue
    /// @return true if successful, false if queue is empty
    [[nodiscard]] auto constexpr try_pop(T& item) noexcept -> bool
    {
        const size_t current_head = head_.load(std::memory_order_relaxed);

        // Check if queue is empty using acquire ordering to synchronize with producer
        if (current_head == tail_.load(std::memory_order_acquire)) [[unlikely]] {
            return false;
        }

        // Load the item
        if constexpr (std::is_trivially_copyable_v<T>) {
            item = slots_[current_head & MASK].data;
        } else {
            item = std::move(slots_[current_head & MASK].data);
            std::destroy_at(&slots_[current_head & MASK].data);
        }

        // Update head position with release ordering
        head_.store((current_head + 1) & MASK, std::memory_order_release);
        return true;
    }

    /// @brief Check if queue is empty (approximate - may be stale)
    /// @remarks Use only for monitoring, not for control flow
    /// @return true if the queue is empty, false otherwise
    [[nodiscard]] auto constexpr empty() const noexcept -> bool
    {
        return head_.load(std::memory_order_relaxed) == tail_.load(std::memory_order_relaxed);
    }

    /// @brief Get approximate size (may be stale)
    /// @remarks Use only for monitoring, not for control flow
    /// @return The size of the queue
    [[nodiscard]] auto constexpr size() const noexcept -> size_t
    {
        const size_t tail = tail_.load(std::memory_order_relaxed);
        const size_t head = head_.load(std::memory_order_relaxed);
        return (tail - head) & MASK;
    }

    /// @brief Ge the queues capacity
    /// @return
    [[nodiscard]] constexpr auto capacity() const noexcept -> size_t { return Capacity; }
};
}  // namespace lockfree::queue
