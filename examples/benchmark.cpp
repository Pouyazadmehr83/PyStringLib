#include "pystring/pystring.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

int main() {
    using namespace pystring;

    std::cout << "===============================================\n";
    std::cout << "       PyStringLib: Performance Benchmark      \n";
    std::cout << "===============================================\n\n";

    constexpr size_t ITERATIONS = 100'000;
    std::string sample = "  https://github.com/Pouyazadmehr83/PyStringLib:Modern-Cpp-String-Utilities  ";

    // 1. Strip + Split Benchmark
    {
        auto t0 = std::chrono::high_resolution_clock::now();
        size_t token_count = 0;
        for (size_t i = 0; i < ITERATIONS; ++i) {
            String s(sample);
            auto tokens = s.strip().split("/");
            token_count += tokens.size();
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        std::cout << "1. String class (strip + split):        "
                  << std::fixed << std::setprecision(2) << ms << " ms (" 
                  << (ITERATIONS * 1000.0 / ms) << " ops/sec)\n";
    }

    // 2. Zero-Copy StringView Split Benchmark
    {
        auto t0 = std::chrono::high_resolution_clock::now();
        size_t token_count = 0;
        for (size_t i = 0; i < ITERATIONS; ++i) {
            std::string_view sv = sample;
            auto tokens = split_view(strip_view(sv), "/");
            token_count += tokens.size();
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        std::cout << "2. Zero-Copy View (strip_view + split):  "
                  << std::fixed << std::setprecision(2) << ms << " ms (" 
                  << (ITERATIONS * 1000.0 / ms) << " ops/sec)\n";
    }

    // 3. Slicing Benchmark
    {
        auto t0 = std::chrono::high_resolution_clock::now();
        size_t char_count = 0;
        String s = sample;
        for (size_t i = 0; i < ITERATIONS; ++i) {
            String sub = s(10, 40, 2);
            char_count += sub.size();
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        std::cout << "3. Pythonic Slicing (100k slices):       "
                  << std::fixed << std::setprecision(2) << ms << " ms (" 
                  << (ITERATIONS * 1000.0 / ms) << " ops/sec)\n";
    }

    // 4. Join Benchmark
    {
        std::vector<std::string> parts = {"C++", "Python", "Rust", "Go", "TypeScript", "Kotlin", "Swift"};
        auto t0 = std::chrono::high_resolution_clock::now();
        size_t total_len = 0;
        for (size_t i = 0; i < ITERATIONS; ++i) {
            std::string j = join(" | ", parts);
            total_len += j.size();
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        std::cout << "4. Templated Join (100k joins):          "
                  << std::fixed << std::setprecision(2) << ms << " ms (" 
                  << (ITERATIONS * 1000.0 / ms) << " ops/sec)\n";
    }

    std::cout << "\n[✓] Benchmark completed successfully.\n";
    return 0;
}
