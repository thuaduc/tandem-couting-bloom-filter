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
#include <vector>
#include <time.h>

#include <doctest/doctest.h>

#include "bf.hpp"
#include "cbf.hpp"
#include "tbf.hpp"
#include "vbf.hpp"

size_t lookup_range = 100000;
size_t filter_length_bits = 16384;
uint8_t k_hash_functions = 4;
uint8_t l_set = 8;
size_t repetitions = 1;

std::array<int, 6> elementsInSet = {410, 341, 292, 256, 228, 205};
std::array<size_t, 6> bitsPerElement = {40, 48, 56, 64, 72, 80};

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

void printBenchmark(double insertTime, double lookupTime, double removeTime)
{
    std::cout << "Insert time: " << insertTime << " (Miliseconds)" << std::endl;
    std::cout << "Lookup time: " << lookupTime << " (Miliseconds)" << std::endl;

    if (removeTime != 0)
    {
        std::cout << "Remove time: " << removeTime << " (Miliseconds)"
                  << std::endl;
    }
}

template <typename T>
std::tuple<double, double, double> runTestSameMemory(
    T &bf, const std::vector<std::vector<uint8_t>> &data)
{
    // Benchmark insert
    double startInsert = currentTime();
    for (auto line : data)
    {
        bf.insert(line.data(), line.size());
    }
    double insertTime = currentTime() - startInsert;

    // Benchmark lookup
    double startLookup = currentTime();
    for (auto line : data)
    {
        bf.lookup(line.data(), line.size());
    }
    double lookupTime = currentTime() - startLookup;

    double startRemove = currentTime();
    double removeTime = 0;

    // Benchmark remove
    for (auto line : data)
    {
        bf.remove(line.data(), line.size());
    }
    removeTime = currentTime() - startRemove;

    return std::make_tuple(insertTime, lookupTime, removeTime);
}

std::pair<double, double> runTestSameMemory_BF(
    BloomFilter &bf, const std::vector<std::vector<uint8_t>> &data)
{
    // Benchmark insert
    double startInsert = currentTime();
    for (auto line : data)
    {
        bf.insert(line.data(), line.size());
    }
    double insertTime = currentTime() - startInsert;

    // Benchmark lookup
    double startLookup = currentTime();
    for (auto line : data)
    {
        bf.lookup(line.data(), line.size());
    }
    double lookupTime = currentTime() - startLookup;

    return std::make_pair(insertTime, lookupTime);
}

template <typename T>
double FPR_Removal(T &bf, std::vector<std::vector<uint8_t>> &data,
                   size_t elesInSet)
{
    double falsePositiveRate = 0;

    for (size_t i = 0; i < elesInSet; ++i)
    {
        bf.insert(data.at(i).data(), data.at(i).size());
    }

    for (size_t i = elesInSet; i < elesInSet * 2; ++i)
    {
        bf.insert(data.at(i).data(), data.at(i).size());
    }

    for (size_t i = elesInSet; i < elesInSet * 2; ++i)
    {
        bf.remove(data.at(i).data(), data.at(i).size());
    }

    for (size_t i = elesInSet; i < elesInSet + lookup_range; i++)
    {
        if (bf.lookup(data.at(i).data(), data.at(i).size()))
        {
            ++falsePositiveRate;
        }
    }
    return (100.0 * falsePositiveRate) / lookup_range;
}
