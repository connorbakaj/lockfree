#include <catch2/catch_test_macros.hpp>

#include "lockfree/spsc.h"

TEST_CASE("Basic push/pop", "[spsc]")
{
    lockfree::queue::Spsc<int, 1024> queue;

    REQUIRE(queue.try_push(42));
    auto value = 0;
    REQUIRE(queue.try_pop(value));
    REQUIRE(value == 42);
}

TEST_CASE("Push fails when queue full", "[spsc]")
{
    lockfree::queue::Spsc<int, 4> queue;

    REQUIRE(queue.try_push(42));
    REQUIRE(queue.try_push(43));
    REQUIRE(queue.try_push(44));
    
    REQUIRE(!queue.try_push(45));
}

TEST_CASE("Push successful after queue popped", "[spsc]")
{
    lockfree::queue::Spsc<int, 4> queue;

    REQUIRE(queue.try_push(42));
    REQUIRE(queue.try_push(43));
    REQUIRE(queue.try_push(44));

    REQUIRE(!queue.try_push(45));

    auto value = 0;
    REQUIRE(queue.try_pop(value));

    REQUIRE(queue.try_push(45));
}

TEST_CASE("Pop fails on empty queue", "[spsc]")
{
    lockfree::queue::Spsc<int, 4> queue;

    auto value = 0;
    REQUIRE(!queue.try_pop(value));
}
