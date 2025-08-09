#pragma once

#include <chrono>
#include <cstddef>
#include <iostream>
#include <thread>

namespace benchmark
{

/// @brief Simple benchmark for measuring queue performance
/// @tparam Queue The queue to benchmark
template <typename Queue>
void benchmark_queue(std::string_view queue_name, size_t iterations = 1000000)
{
    Queue queue;
    std::atomic<bool> start{false};
    std::atomic<bool> producer_done{false};

    std::cout << "\n--- Benchmark for " << queue_name << " ---\n";

    auto producer = std::thread([&]() {
        while (!start.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }

        auto start_time = std::chrono::high_resolution_clock::now();

        for (size_t i = 0; i < iterations; ++i) {
            while (!queue.try_push(static_cast<int>(i))) {
                std::this_thread::yield();
            }
        }

        producer_done.store(true, std::memory_order_release);

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

        std::cout << "Producer: " << duration << "ns & " << duration / iterations << " ns/op"
                  << '\n';
    });

    auto consumer = std::thread([&]() {
        while (!start.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }

        auto start_time = std::chrono::high_resolution_clock::now();
        size_t consumed = 0;
        int item = 0;

        while (consumed < iterations) {
            if (queue.try_pop(item)) {
                ++consumed;
            } else if (producer_done.load(std::memory_order_acquire)) {
                // Producer finished, do one final drain
                while (queue.try_pop(item)) {
                    ++consumed;
                }
                break;
            } else {
                std::this_thread::yield();
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::cout << "Consumer: " << duration << "ns & " << duration / consumed
                  << " ns/op, items: " << consumed << '\n';
    });

    // Start benchmark
    start.store(true, std::memory_order_release);

    producer.join();
    consumer.join();
}

}  // namespace benchmark
