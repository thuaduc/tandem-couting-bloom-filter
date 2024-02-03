#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <type_traits>
#include <vector>

#include "bf.hpp"

size_t lookup_range = 100000;
size_t in_set_high = 10000;
size_t out_set_low = 15000;
size_t urls_length = 6391377;
size_t filter_length_bits = 32768;
uint8_t k_hash_functions = 5;
uint8_t l_set = 4;
size_t repetitions = 10;

std::array<int, 6> elementsInSet = {820, 683, 586, 512, 456, 410};
std::array<size_t, 6> bitsPerElement_TBF = {40, 48, 56, 64, 72, 80};
std::array<size_t, 6> bitsPerElement_CBF = {80, 96, 112, 128, 144, 160};

double currentTime() {
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(
               now.time_since_epoch())
               .count() *
           1e-3;
}

std::vector<uint8_t> stringToVector(const std::string &str) {
    return std::vector<uint8_t>(str.begin(), str.end());
}

size_t randomNumberGenerator(int a, int b) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(a, b);
    int randomNumber = distribution(generator);
    return static_cast<size_t>(randomNumber);
}

void printBenchmark(double insertTime, double lookupTime, double removeTime) {
    std::cout << "Insert time: " << insertTime << " (Miliseconds)" << std::endl;
    std::cout << "Lookup time: " << lookupTime << " (Miliseconds)" << std::endl;

    if (removeTime != 0) {
        std::cout << "Remove time: " << removeTime << " (Miliseconds)"
                  << std::endl;
    }
}

template <typename T>
std::tuple<double, double, double> runTestSameMemory(
    T &bf, const std::vector<std::vector<uint8_t>> &data) {
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

    double startRemove = currentTime();
    double removeTime = 0;

    // Benchmark remove
    for (auto line : data) {
        bf.remove(line.data(), line.size());
    }
    removeTime = currentTime() - startRemove;

    return std::make_tuple(insertTime, lookupTime, removeTime);
}

std::pair<double, double> runTestSameMemory_BF(
    BloomFilter &bf, const std::vector<std::vector<uint8_t>> &data) {
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

    return std::make_pair(insertTime, lookupTime);
}

template <typename T>
double FPR_Removal(T &bf, std::vector<std::vector<uint8_t>> &data,
                   size_t elesInSet) {
    double falsePositiveRate = 0;
    auto in_set_low_bound = randomNumberGenerator(0, in_set_high);
    auto out_set_low_bound =
        randomNumberGenerator(out_set_low, urls_length - lookup_range);

    for (size_t i = in_set_low_bound; i < in_set_low_bound + elesInSet; ++i) {
        bf.insert(data.at(i).data(), data.at(i).size());
    }

    // for (size_t i = out_set_low_bound; i < out_set_low_bound + lookup_range;
    //      ++i) {
    //     bf.insert(data.at(i).data(), data.at(i).size());
    //     bf.remove(data.at(i).data(), data.at(i).size());
    // }

    // for (size_t i = out_set_low_bound; i < out_set_low_bound + lookup_range;
    //      ++i) {
    // }

    for (size_t i = out_set_low_bound; i < out_set_low_bound + lookup_range;
         ++i) {
        if (bf.lookup(data.at(i).data(), data.at(i).size())) {
            ++falsePositiveRate;
        }
    }
    return (100.0 * falsePositiveRate) / lookup_range;
}
