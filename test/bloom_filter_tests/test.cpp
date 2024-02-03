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

size_t lookup_range = 20000;
size_t in_set_high = 10000;
size_t out_set_low = 15000;
size_t urls_length = 6391377;
size_t filter_length_bits = 32768;
uint8_t k_hash_functions = 5;
uint8_t l_set = 8;
size_t repetitions = 10;

std::array<int, 6> elementsInSet = {820, 683, 586, 512, 456, 410};
std::array<size_t, 6> bitsPerElement_TBF = {40, 48, 56, 64, 72, 80};
std::array<size_t, 6> bitsPerElement_CBF = {80, 96, 112, 128, 144, 160};

double currentTime()
{
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(
               now.time_since_epoch())
               .count() *
           1e-3;
}

std::vector<uint8_t> stringToVector(const std::string &str)
{
    return std::vector<uint8_t>(str.begin(), str.end());
}

size_t randomNumberGenerator(int a, int b)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(a, b);
    int randomNumber = distribution(generator);
    return static_cast<size_t>(randomNumber);
}

void printBenchmark(double insertTime, double lookupTime, double removeTime,
                    double falsePositiveRate)
{
    std::cout << "Insert time: " << insertTime << " (Miliseconds)" << std::endl;
    std::cout << "Lookup time: " << lookupTime << " (Miliseconds)" << std::endl;

    if (removeTime != 0)
    {
        std::cout << "Remove time: " << removeTime << " (Miliseconds)"
                  << std::endl;
    }

    if (falsePositiveRate != 0)
    {
        std::cout << "False positive rate: " << falsePositiveRate << "%" << std::endl;
    }
}

template <typename T>
void runTestTrueNegative(T &bf, const std::vector<std::vector<uint8_t>> &data)
{
    for (auto line : data)
    {
        bf.insert(line.data(), line.size());
    }

    for (auto line : data)
    {
        CHECK_EQ(bf.lookup(line.data(), line.size()), true);
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

template <typename T>
double FPR_Removal(T &bf, std::vector<std::vector<uint8_t>> &data, size_t elesInSet)
{
    double falsePositiveRate = 0;
    auto in_set_low_bound = randomNumberGenerator(0, in_set_high);
    auto out_set_low_bound = randomNumberGenerator(out_set_low, urls_length - lookup_range);

    for (size_t i = in_set_low_bound; i < in_set_low_bound + elesInSet; ++i)
    {
        bf.insert(data.at(i).data(), data.at(i).size());
    }

    // for (size_t i = out_set_low_bound; i < out_set_low_bound + lookup_range; ++i)
    // {
    //     bf.insert(data.at(i).data(), data.at(i).size());
    //     bf.remove(data.at(i).data(), data.at(i).size());
    // }

    for (size_t i = out_set_low_bound; i < out_set_low_bound + lookup_range; ++i)
    {
        if (bf.lookup(data.at(i).data(), data.at(i).size()))
        {
            ++falsePositiveRate;
        }
    }
    return (100.0 * falsePositiveRate) / lookup_range;
}

TEST_CASE("Benchmarking false positive rate")
{
    srand(42);
    std::vector<std::vector<uint8_t>> data;
    std::ifstream in(getenv("FILE"));
    std::string line;
    while (getline(in, line))
    {
        data.push_back(stringToVector(line));
    };

    SUBCASE("Case no removal Counting Bloomfilter")
    {
        std::cout << "-------------" << std::endl;
        for (size_t i = 0; i < bitsPerElement_TBF.size(); ++i)
        {
            double cbf_FPR = 0;

            for (size_t j = 0; j < repetitions; ++j)
            {
                CountingBloomFilter cbf{filter_length_bits, k_hash_functions};
                cbf_FPR += FPR_Removal<CountingBloomFilter>(
                    cbf, data, elementsInSet.at(i) * 2);
            }

            std::cout << "False Positive Rate Counting BloomFilter with "
                      << bitsPerElement_TBF.at(i)
                      << " bits per elements: " << std::fixed << std::setprecision(6) << (cbf_FPR / repetitions) << std::endl;
        }
        std::cout << "-------------" << std::endl;
    }

    SUBCASE("Case no removal Variable Counting Bloomfilter")
    {
        for (size_t i = 0; i < bitsPerElement_TBF.size(); ++i)
        {
            double vbf_FPR = 0;

            for (size_t j = 0; j < repetitions; ++j)
            {
                VariableCoutingBloomFilter vbf{filter_length_bits, k_hash_functions, l_set};
                vbf_FPR += FPR_Removal<VariableCoutingBloomFilter>(
                    vbf, data, elementsInSet.at(i));
            }

            std::cout << "False Positive Rate VariableCoutingBloomFilter with "
                      << bitsPerElement_TBF.at(i)
                      << " bits per elements: " << std::fixed << std::setprecision(6) << (vbf_FPR / repetitions) << std::endl;
        }
        std::cout << "-------------" << std::endl;
    }

    SUBCASE("Case no removal Tandem Counting Bloomfilter")
    {
        for (size_t i = 0; i < bitsPerElement_TBF.size(); ++i)
        {
            double tbf_FPR = 0;

            for (size_t j = 0; j < repetitions; ++j)
            {
                TandemBloomFilter tbf{filter_length_bits, k_hash_functions, l_set};
                tbf_FPR += FPR_Removal<TandemBloomFilter>(
                    tbf, data, elementsInSet.at(i));
            }

            std::cout << "False Positive Rate TandemBloomFilter with "
                      << bitsPerElement_TBF.at(i)
                      << " bits per elements: " << std::fixed << std::setprecision(6) << (tbf_FPR / repetitions) << std::endl;
        }
        std::cout << "-------------" << std::endl;
    }
}
