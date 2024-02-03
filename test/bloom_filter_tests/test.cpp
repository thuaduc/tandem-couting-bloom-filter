#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "test.hpp"

#include <doctest/doctest.h>
#include <time.h>

#include <vector>

#include "bf.hpp"
#include "cbf.hpp"
#include "tbf.hpp"
#include "vbf.hpp"

TEST_CASE("Benchmarking false positive rate") {
    srand(42);
    std::vector<std::vector<uint8_t>> data;
    std::ifstream in(getenv("FILE"));
    std::string line;
    while (getline(in, line)) {
        data.push_back(stringToVector(line));
    };

    SUBCASE("Case no removal Counting Bloomfilter") {
        std::cout << "-------------" << std::endl;
        for (size_t i = 0; i < bitsPerElement_TBF.size(); ++i) {
            double cbf_FPR = 0;

            for (size_t j = 0; j < repetitions; ++j) {
                CountingBloomFilter cbf{filter_length_bits, k_hash_functions};
                cbf_FPR += FPR_Removal<CountingBloomFilter>(
                    cbf, data, elementsInSet.at(i) * 2);
            }

            std::cout << "False Positive Rate Counting BloomFilter with "
                      << bitsPerElement_TBF.at(i)
                      << " bits per elements: " << std::fixed
                      << std::setprecision(6) << (cbf_FPR / repetitions)
                      << std::endl;
        }
        std::cout << "-------------" << std::endl;
    }

    SUBCASE("Case no removal Variable Counting Bloomfilter") {
        for (size_t i = 0; i < bitsPerElement_TBF.size(); ++i) {
            double vbf_FPR = 0;

            for (size_t j = 0; j < repetitions; ++j) {
                VariableCoutingBloomFilter vbf{filter_length_bits,
                                               k_hash_functions, l_set};
                vbf_FPR += FPR_Removal<VariableCoutingBloomFilter>(
                    vbf, data, elementsInSet.at(i));
            }

            std::cout << "False Positive Rate VariableCoutingBloomFilter with"
                      << bitsPerElement_TBF.at(i)
                      << " bits per elements: " << std::fixed
                      << std::setprecision(6) << (vbf_FPR / repetitions)
                      << std::endl;
        }
        std::cout << "-------------" << std::endl;
    }

    SUBCASE("Case no removal Tandem Counting Bloomfilter") {
        for (size_t i = 0; i < bitsPerElement_TBF.size(); ++i) {
            double tbf_FPR = 0;

            for (size_t j = 0; j < repetitions; ++j) {
                TandemBloomFilter tbf{filter_length_bits, k_hash_functions,
                                      l_set};
                tbf_FPR += FPR_Removal<TandemBloomFilter>(tbf, data,
                                                          elementsInSet.at(i));
            }

            std::cout << "False Positive Rate TandemBloomFilter with "
                      << bitsPerElement_TBF.at(i)
                      << " bits per elements: " << std::fixed
                      << std::setprecision(6) << (tbf_FPR / repetitions)
                      << std::endl;
        }
        std::cout << "-------------" << std::endl;
    }
}

// TEST_CASE("Basic test true negaive") {
//     std::vector<std::vector<uint8_t>> data;
//     std::vector<uint64_t> v;
//     for (uint64_t i = 0; i < 1000000; ++i) v.push_back(i);
//     for (auto x : v) {
//         union {
//             uint32_t x;
//             uint8_t bytes[4];
//         } u;
//         u.x = x;
//         data.emplace_back(u.bytes, u.bytes + 4);
//     }

//     SUBCASE("Bloomfilter") {
//         BloomFilter filter{data.size(), 4};
//         runTestTrueNegative<BloomFilter>(filter, data);
//     }
//     SUBCASE("Couting Bloomfilter") {
//         CountingBloomFilter filter{data.size(), 4};
//         runTestTrueNegative<CountingBloomFilter>(filter, data);
//     }
//     SUBCASE("Variable Couting Bloomfilter") {
//         VariableCoutingBloomFilter filter{data.size(), 4, 8};
//         runTestTrueNegative<VariableCoutingBloomFilter>(filter, data);
//     }
//     SUBCASE("Tandem Bloomfilter") {
//         TandemBloomFilter filter{data.size(), 4, 8};
//         runTestTrueNegative<TandemBloomFilter>(filter, data);
//     }
// }

// TEST_CASE("Run performance test same memory") {
//     std::vector<std::vector<uint8_t>> data;

//     std::vector<uint64_t> v;
//     for (uint64_t i = 0; i < 10000000; ++i) v.push_back(i);
//     for (auto x : v) {
//         union {
//             uint32_t x;
//             uint8_t bytes[4];
//         } u;
//         u.x = x;
//         data.emplace_back(u.bytes, u.bytes + 4);
//     }

//     SUBCASE("Bloomfilter") {
//         std::cout << std::endl
//                   << "Benchmarking Bloomfilter Same Memory " << std::endl;
//         BloomFilter filter{data.size(), k_hash_functions};
//         auto [insertTime, lookupTime] = runTestSameMemory_BF(filter, data);
//         printBenchmark(insertTime, lookupTime, 0);
//     }

//     SUBCASE("Counting Bloomfilter") {
//         std::cout << std::endl
//                   << "Benchmarking Couting Bloomfilter Same Memory "
//                   << std::endl;
//         CountingBloomFilter filter{data.size(), k_hash_functions};
//         auto [insertTime, lookupTime, removeTime] =
//             runTestSameMemory<CountingBloomFilter>(filter, data);
//         printBenchmark(insertTime, lookupTime, removeTime);
//     }

//     SUBCASE("Variable Couting Bloomfilter") {
//         std::cout << std::endl
//                   << "Benchmarking Variable Couting Bloomfilter Same Memory "
//                   << std::endl;
//         VariableCoutingBloomFilter filter{data.size(), k_hash_functions,
//         l_set}; auto [insertTime, lookupTime, removeTime] =
//             runTestSameMemory<VariableCoutingBloomFilter>(filter, data);
//         printBenchmark(insertTime, lookupTime, removeTime);
//     }

//     SUBCASE("Tandem Bloomfilter") {
//         std::cout << std::endl
//                   << "Benchmarking Tandem Bloomfilter Same Memory "
//                   << std::endl;
//         TandemBloomFilter filter{data.size(), k_hash_functions, l_set};
//         auto [insertTime, lookupTime, removeTime] =
//             runTestSameMemory<TandemBloomFilter>(filter, data);
//         printBenchmark(insertTime, lookupTime, removeTime);
//     }
// }
