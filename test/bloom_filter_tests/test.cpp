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
#include <doctest/doctest.h>


#include "bf.hpp"
#include "cbf.hpp"
#include "tbf.hpp"
#include "vbf.hpp"

#define filterLengthInBits uint64_t 2e15
#define kHashFunctions 4
#define L_set 8

std::array<size_t, 8> elementsInSet = {1024, 820, 683, 586, 512, 456, 410};
std::array<size_t, 8> bitsPerElement_TBF = {32, 40, 48, 56, 64, 72, 80};
std::array<size_t, 8> bitsPerElement_CBF = {64, 80, 96, 112, 128, 144, 160};

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

int randomNumberGenerator() {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, 3999);
    int randomNumber = distribution(generator);
    return randomNumber;
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
        std::cout << "False positive rate: " << falsePositiveRate << "%"<< std::endl;
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

template <typename T>
double FPR_NoRemoval(T& bf, std::vector<std::vector<uint8_t>>& dataInSet,
                     std::vector<std::vector<uint8_t>>& dataNotInSet,
                     size_t elesInSet) {
    double falsePositiveRate = 0;
    auto ranNum = randomNumberGenerator();

    for (size_t i = 0; i < elesInSet; ++i) {
        bf.insert(dataInSet.at(i).data(), dataInSet.at(i).size());
    }

    for (auto line : dataNotInSet) {
        if (bf.lookup(line.data(), line.size())) {
            ++falsePositiveRate;
        }
    }

    return (100 * falsePositiveRate) / elesInSet;
}

TEST_CASE("Benchmarking false positive rate") {
    std::vector<std::vector<uint8_t>> dataInSet;
    std::vector<std::vector<uint8_t>> dataNotInSet;

    std::vector<uint64_t> v;
    for (uint64_t i = 0; i < 10000; ++i) v.push_back(i);
    size_t c = 0;
    for (auto x : v) {
        union {
            uint32_t x;
            uint8_t bytes[4];
        } u;
        u.x = x;
        dataInSet.emplace_back(u.bytes, u.bytes + 4);
        u.x = x + 10000;
        dataNotInSet.emplace_back(u.bytes, u.bytes + 4);
    }

    SUBCASE("Case no removal couting bloom filter") {
        std::cout << "-------------" << std::endl;
        for (int i = 0; i < 7; ++i) {
            double cbf_FPR = 0;

            for (int j = 0; j < 10; j++) {
                CountingBloomFilter cbf{32768, 4};
                cbf_FPR += FPR_NoRemoval<CountingBloomFilter>(
                    cbf, dataInSet, dataNotInSet, elementsInSet.at(i));
            }

            std::cout << "False Positive Rate Counting BloomFilter with "
                      << elementsInSet.at(i)
                      << " elements in Set: " << cbf_FPR / 10 << std::endl;
        }
        std::cout << "-------------" << std::endl;
    }

    SUBCASE("Case no removal variable couting bloom filter") {
        for (int i = 0; i < 7; ++i) {
            double vbf_FPR = 0;

            for (int j = 0; j < 10; j++) {
                VariableCoutingBloomFilter vbf{32768, 4, 8};
                vbf_FPR += FPR_NoRemoval<VariableCoutingBloomFilter>(
                    vbf, dataInSet, dataNotInSet, elementsInSet.at(i));
            }

            std::cout << "False Positive Rate VariableCoutingBloomFilter with "
                      << elementsInSet.at(i)
                      << " elements in Set: " << vbf_FPR / 10 << std::endl;
        }
        std::cout << "-------------" << std::endl;
    }

    SUBCASE("Case no removal tandem couting bloom filter") {
        for (int i = 0; i < 7; ++i) {
            double tbf_FPR = 0;

            for (int j = 0; j < 10; j++) {
                TandemBloomFilter tbf{32768, 4, 8};
                tbf_FPR += FPR_NoRemoval<TandemBloomFilter>(
                    tbf, dataInSet, dataNotInSet, elementsInSet.at(i));
            }

            std::cout << "False Positive Rate TandemBloomFilter with "
                      << elementsInSet.at(i)
                      << " elements in Set: " << tbf_FPR / 10 << std::endl;
        }
        std::cout << "-------------" << std::endl;
    }
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

// TEST_CASE("Run performance test and false positive test") {
//     srand(42);
//     std::vector<std::vector<uint8_t>> data;
//     std::vector<std::vector<uint8_t>> shuffledData;

//     std::ifstream in(getenv("FILE"));
//     std::string line;
//     while (getline(in, line)) {
//         auto v = stringToVector(line);
//         data.push_back(v);

//         auto shuffled = v;
//         std::random_device rd;
//         std::mt19937 g(rd());
//         std::shuffle(shuffled.begin(), shuffled.end(), g);
//         shuffledData.push_back(shuffled);
//     };

//     // SUBCASE("Bloomfilter") {
//     //     BloomFilter filter{data.size(), 4};
//     //     runTestWithFalsePositive<BloomFilter>(filter, data);
//     // }

//     SUBCASE("Counting Bloomfilter") {
//         std::cout << std::endl
//                   << "Benchmarking Couting Bloomfilter " << std::endl;
//         CountingBloomFilter filter{data.size(), 4};
//         auto [insertTime, lookupTime, removeTime, falsePositiveRate] =
//             runTestWithFalsePositive<CountingBloomFilter>(filter, data,
//                                                           shuffledData);
//         printBenchmark(insertTime, lookupTime, removeTime,
//         falsePositiveRate);
//     }

//     SUBCASE("Variable Couting Bloomfilter") {
//         std::cout << std::endl
//                   << "Benchmarking Variable Couting Bloomfilter " <<
//                   std::endl;
//         VariableCoutingBloomFilter filter{data.size(), 4, 8};
//         auto [insertTime, lookupTime, removeTime, falsePositiveRate] =
//             runTestWithFalsePositive<VariableCoutingBloomFilter>(filter,
//             data,
//                                                                  shuffledData);
//         printBenchmark(insertTime, lookupTime, removeTime,
//         falsePositiveRate);
//     }

//     SUBCASE("Tandem Bloomfilter") {
//         std::cout << std::endl
//                   << "Benchmarking Tandem Bloomfilter " << std::endl;
//         TandemBloomFilter filter{data.size(), 4, 8};
//         auto [insertTime, lookupTime, removeTime, falsePositiveRate] =
//             runTestWithFalsePositive<TandemBloomFilter>(filter, data,
//                                                         shuffledData);
//         printBenchmark(insertTime, lookupTime, removeTime,
//         falsePositiveRate);
//     }
// }

// TEST_CASE("Run performance test same memory") {
//     std::vector<std::vector<uint8_t>> data;
//     std::vector<std::vector<uint8_t>> bigData;

//     std::vector<uint64_t> v;
//     for (uint64_t i = 0; i < 10000000; ++i) v.push_back(i);
//     for (auto x : v) {
//         union {
//             uint32_t x;
//             uint8_t bytes[4];
//         } u;
//         u.x = x;
//         data.emplace_back(u.bytes, u.bytes + 4);
//         bigData.emplace_back(u.bytes, u.bytes + 4);
//     }

//     v.clear();
//     for (uint64_t i = 10000000; i < 20000000; ++i) v.push_back(i);
//     for (auto x : v) {
//         union {
//             uint32_t x;
//             uint8_t bytes[4];
//         } u;
//         u.x = x;
//         bigData.emplace_back(u.bytes, u.bytes + 4);
//     }

//     CHECK_EQ(bigData.size(), 2 * data.size());

//     SUBCASE("Counting Bloomfilter") {
//         std::cout << std::endl
//                   << "Benchmarking Couting Bloomfilter Same Memory "
//                   << std::endl;
//         CountingBloomFilter filter{bigData.size(), 4};
//         auto [insertTime, lookupTime, removeTime] =
//             runTestSameMemory<CountingBloomFilter>(filter, bigData);
//         printBenchmark(insertTime, lookupTime, removeTime, 0);
//     }

//     SUBCASE("Variable Couting Bloomfilter") {
//         std::cout << std::endl
//                   << "Benchmarking Variable Couting Bloomfilter Same
//                   Memory "
//                   << std::endl;
//         VariableCoutingBloomFilter filter{data.size(), 4, 8};
//         auto [insertTime, lookupTime, removeTime] =
//             runTestSameMemory<VariableCoutingBloomFilter>(filter, data);
//         printBenchmark(insertTime, lookupTime, removeTime, 0);
//     }

//     SUBCASE("Tandem Bloomfilter") {
//         std::cout << std::endl
//                   << "Benchmarking Tandem Bloomfilter Same Memory "
//                   << std::endl;
//         TandemBloomFilter filter{data.size(), 4, 8};
//         auto [insertTime, lookupTime, removeTime] =
//             runTestSameMemory<TandemBloomFilter>(filter, data);
//         printBenchmark(insertTime, lookupTime, removeTime, 0);
//     }
// }
