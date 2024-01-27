#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <time.h>

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <map>
#include <random>
#include <string>
#include <type_traits>
#include <vector>

#include "bf.hpp"
#include "cbf.hpp"
#include "doctest/doctest/doctest.h"
#include "tbf.hpp"
#include "vbf.hpp"

double currentTime() {
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(
               now.time_since_epoch())
               .count() *
           1e-3;
}

std::vector<uint8_t> stringToVector(const std::string& str) {
    return std::vector<uint8_t>(str.begin(), str.end());
}

std::vector<uint8_t> shuffleVector(std::vector<uint8_t>& input) {
    std::vector<uint8_t> shuffledVector = input;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(shuffledVector.begin(), shuffledVector.end(), g);
    return shuffledVector;
}

void printBenchmark(double insertTime, double lookupTime, double removeTime,
                    double falsePositiveRate) {
    std::cout << "Insert time: " << insertTime << " (Miliseconds)" << std::endl;
    std::cout << "Lookup time: " << lookupTime << " (Miliseconds)" << std::endl;

    if (removeTime != 0) {
        std::cout << "Remove time: " << removeTime << " (Miliseconds)"
                  << std::endl;
    }

    if (falsePositiveRate != 0) {
        std::cout << "False positive rate: " << falsePositiveRate << std::endl;
    }
}

template <typename T>
void runTestTrueNegative(T& bf, const std::vector<std::vector<uint8_t>>& data) {
    for (auto line : data) {
        bf.insert(line.data(), line.size());
    }

    for (auto line : data) {
        CHECK_EQ(bf.lookup(line.data(), line.size()), true);
    }
}

// insert time, lookup time, delete time, false positive rate
template <typename T>
std::tuple<double, double, double, double> runTestWithFalsePositive(
    T& bf, const std::vector<std::vector<uint8_t>>& data,
    std::vector<std::vector<uint8_t>>& shuffledData) {
    double falsePositiveRate = 0;
    // Benchmark insert
    double startInsert = currentTime();
    for (auto line : data) {
        bf.insert(line.data(), line.size());
    }
    double insertTime = currentTime() - startInsert;

    // Benchmark lookup
    double startLookup = currentTime();
    for (auto line : data) {
        bf.lookup(line.data(), line.size());
    }
    double lookupTime = currentTime() - startLookup;

    // Check False positive
    for (auto line : shuffledData) {
        if (bf.lookup(line.data(), line.size())) {
            ++falsePositiveRate;
        }
    }

    falsePositiveRate = (100 * falsePositiveRate) / data.size();

    if (!std::is_same<T, BloomFilter>::value) {
        // Benchmark remove
        double startRemove = currentTime();
        for (auto line : data) {
            bf.remove(line.data(), line.size());
        }
        double removeTime = currentTime() - startRemove;
        return std::make_tuple(insertTime, lookupTime, removeTime,
                               falsePositiveRate);
    } else {
        return std::make_tuple(insertTime, lookupTime, 0, falsePositiveRate);
    }
}

template <typename T>
std::tuple<double, double, double> runTestSameMemory(
    T& bf, const std::vector<std::vector<uint8_t>>& data) {
    // Benchmark insert
    double startInsert = currentTime();
    for (auto line : data) {
        bf.insert(line.data(), line.size());
    }
    double insertTime = currentTime() - startInsert;

    // Benchmark lookup
    double startLookup = currentTime();
    for (auto line : data) {
        bf.lookup(line.data(), line.size());
    }
    double lookupTime = currentTime() - startLookup;

    // Benchmark remove
    double startRemove = currentTime();
    for (auto line : data) {
        bf.remove(line.data(), line.size());
    }
    double removeTime = currentTime() - startRemove;

    return std::make_tuple(insertTime, lookupTime, removeTime);
}

TEST_CASE("Check true negaive") {
    std::vector<std::vector<uint8_t>> data;
    std::vector<uint64_t> v;
    for (uint64_t i = 0; i < 1000000; ++i) v.push_back(i);
    for (auto x : v) {
        union {
            uint32_t x;
            uint8_t bytes[4];
        } u;
        u.x = x;
        data.emplace_back(u.bytes, u.bytes + 4);
    }

    SUBCASE("Bloomfilter") {
        BloomFilter filter{data.size(), 4};
        runTestTrueNegative<BloomFilter>(filter, data);
    }
    SUBCASE("Couting Bloomfilter") {
        CountingBloomFilter filter{data.size(), 4};
        runTestTrueNegative<CountingBloomFilter>(filter, data);
    }
    SUBCASE("Variable Couting Bloomfilter") {
        VariableCoutingBloomFilter filter{data.size(), 4, 8};
        runTestTrueNegative<VariableCoutingBloomFilter>(filter, data);
    }
    SUBCASE("Tandem Bloomfilter") {
        TandemBloomFilter filter{data.size(), 4, 8};
        runTestTrueNegative<TandemBloomFilter>(filter, data);
    }
}

TEST_CASE("Run performance test and false positive test") {
    srand(42);
    std::vector<std::vector<uint8_t>> data;
    std::vector<std::vector<uint8_t>> shuffledData;

    std::ifstream in(getenv("FILE"));
    std::string line;
    while (getline(in, line)) {
        auto v = stringToVector(line);
        data.push_back(v);

        auto shuffled = v;
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(shuffled.begin(), shuffled.end(), g);
        shuffledData.push_back(shuffled);
    };

    // SUBCASE("Bloomfilter") {
    //     BloomFilter filter{data.size(), 4};
    //     runTestWithFalsePositive<BloomFilter>(filter, data);
    // }

    SUBCASE("Counting Bloomfilter") {
        std::cout << std::endl
                  << "Benchmarking Couting Bloomfilter " << std::endl;
        CountingBloomFilter filter{data.size(), 4};
        auto [insertTime, lookupTime, removeTime, falsePositiveRate] =
            runTestWithFalsePositive<CountingBloomFilter>(filter, data,
                                                          shuffledData);
        printBenchmark(insertTime, lookupTime, removeTime, falsePositiveRate);
    }

    SUBCASE("Variable Couting Bloomfilter") {
        std::cout << std::endl
                  << "Benchmarking Variable Couting Bloomfilter " << std::endl;
        VariableCoutingBloomFilter filter{data.size(), 4, 8};
        auto [insertTime, lookupTime, removeTime, falsePositiveRate] =
            runTestWithFalsePositive<VariableCoutingBloomFilter>(filter, data,
                                                                 shuffledData);
        printBenchmark(insertTime, lookupTime, removeTime, falsePositiveRate);
    }

    SUBCASE("Tandem Bloomfilter") {
        std::cout << std::endl
                  << "Benchmarking Tandem Bloomfilter " << std::endl;
        TandemBloomFilter filter{data.size(), 4, 8};
        auto [insertTime, lookupTime, removeTime, falsePositiveRate] =
            runTestWithFalsePositive<TandemBloomFilter>(filter, data,
                                                        shuffledData);
        printBenchmark(insertTime, lookupTime, removeTime, falsePositiveRate);
    }
}

TEST_CASE("Run performance test same memory") {
    std::vector<std::vector<uint8_t>> data;
    std::vector<std::vector<uint8_t>> bigData;

    std::vector<uint64_t> v;
    for (uint64_t i = 0; i < 10000000; ++i) v.push_back(i);
    for (auto x : v) {
        union {
            uint32_t x;
            uint8_t bytes[4];
        } u;
        u.x = x;
        data.emplace_back(u.bytes, u.bytes + 4);
        bigData.emplace_back(u.bytes, u.bytes + 4);
    }

    v.clear();
    for (uint64_t i = 10000000; i < 20000000; ++i) v.push_back(i);
    for (auto x : v) {
        union {
            uint32_t x;
            uint8_t bytes[4];
        } u;
        u.x = x;
        bigData.emplace_back(u.bytes, u.bytes + 4);
    }

    CHECK_EQ(bigData.size(), 2 * data.size());

    SUBCASE("Counting Bloomfilter") {
        std::cout << std::endl
                  << "Benchmarking Couting Bloomfilter Same Memory "
                  << std::endl;
        CountingBloomFilter filter{bigData.size(), 4};
        auto [insertTime, lookupTime, removeTime] =
            runTestSameMemory<CountingBloomFilter>(filter, bigData);
        printBenchmark(insertTime, lookupTime, removeTime, 0);
    }

    SUBCASE("Variable Couting Bloomfilter") {
        std::cout << std::endl
                  << "Benchmarking Variable Couting Bloomfilter Same Memory "
                  << std::endl;
        VariableCoutingBloomFilter filter{data.size(), 4, 8};
        auto [insertTime, lookupTime, removeTime] =
            runTestSameMemory<VariableCoutingBloomFilter>(filter, data);
        printBenchmark(insertTime, lookupTime, removeTime, 0);
    }

    SUBCASE("Tandem Bloomfilter") {
        std::cout << std::endl
                  << "Benchmarking Tandem Bloomfilter Same Memory "
                  << std::endl;
        TandemBloomFilter filter{data.size(), 4, 8};
        auto [insertTime, lookupTime, removeTime] =
            runTestSameMemory<TandemBloomFilter>(filter, data);
        printBenchmark(insertTime, lookupTime, removeTime, 0);
    }
}