#include <CLI/CLI.hpp>
#include <iostream>
#include <string>

#include "benchmark/benchmark.h"
#include "benchmark/cv_queue.h"
#include "benchmark/mutex_queue.h"
#include "lockfree/spsc.h"

auto main(int argc, char** argv) -> int
{
    CLI::App app{"Lock-free Queue Benchmark Runner"};

    bool run_spsc = false;
    size_t iterations = 1'000'000;

    app.add_flag("--spsc", run_spsc, "Run SPSC benchmark");
    app.add_option("-i,--iterations", iterations, "Number of iterations to run");

    CLI11_PARSE(app, argc, argv);

    if (!run_spsc) {
        std::cerr << "Error: Please specify at least one benchmark flag: --spsc\n";
        return 1;
    }

    constexpr int CAPACITY = 1024;

    std::cout << "Lock-Free SPSC Queue Benchmark Suite\n";
    std::cout << "=====================================\n\n";

    // CPU and system info
    std::cout << "System Information:\n";
    std::cout << "- CPU cores: " << std::thread::hardware_concurrency() << "\n";
    std::cout << "- Cache line size: 64 bytes (assumed)\n";
    std::cout << "- Compiler: " << __VERSION__ << "\n\n";

    benchmark::benchmark_queue<benchmark::CondVarQueue<int, CAPACITY>>("Condition Variable Queue", iterations);
    benchmark::benchmark_queue<benchmark::MutexQueue<int>>("Mutex Queue", iterations);

    if (run_spsc) {
        benchmark::benchmark_queue<lockfree::queue::Spsc<int, CAPACITY>>("Lockfree Queue", iterations);
    }


    return 0;
}
