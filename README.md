# 🚀 Low-Latency Lock-Free SPSC Queue (C++23)

[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)]()

A **lock-free**, **single-producer single-consumer** queue written in modern **C++23**.  
Designed for **ultra-low-latency** message passing in high-performance and real-time systems.

| Operation         | Median Latency (ns) | Notes                         |
|-------------------|---------------------|-------------------------------|
| Enqueue (push)    | **5**               | ✅ **Lock-free SPSC**         |
| Dequeue (pop)     | **5**               | ✅ **Lock-free SPSC**         |
| Enqueue (push)    | 73                  | Mutex-based SPSC              |
| Dequeue (pop)     | 80                  | Mutex-based SPSC              |
| Enqueue (push)    | 102                 | Condition variable-based SPSC |
| Dequeue (pop)     | 102                 | Condition variable-based SPSC |


---

## ✨ Features
- **Lock-free** design — no mutexes, no blocking
- **Single-producer / single-consumer** optimized
- **Wait-free push/pop** operations (bounded)
- **Cache-line padding** to avoid false sharing
- **Memory-ordering correctness** with `std::memory_order_acquire` / `std::memory_order_release`
- **C++23 Concepts** to enforce type safety
- **Header-only** for easy integration

---

## 📦 Installation

### Using CMake
```bash
cmake -B build -S . -GNinja
cmake --build build
```

---

## 📊 Benchmarks

We use [CLI11](https://github.com/CLIUtils/CLI11) to select benchmarks at runtime.
Example:

```bash
./build/benchmark/benchmark --spsc
```