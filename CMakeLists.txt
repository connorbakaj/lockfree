cmake_minimum_required(VERSION 3.16)
project(lockfree
    VERSION 0.1.0
    DESCRIPTION "Lock-free SPSC queue library"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optionally set Release as default if not set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Add src as a subdirectory (which defines the interface lib)
add_subdirectory(src)

# Optionally add tests if enabled
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Optionally add benchmarks if enabled
option(BUILD_BENCHMARKS "Build benchmarks" ON)
if(BUILD_BENCHMARKS)
    add_subdirectory(benchmark)
endif()
