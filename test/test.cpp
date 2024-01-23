#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <algorithm>
#include <array>
#include <fstream>
#include <random>
#include <string>

#include "../src/filter/bf/bf.hpp"
#include "../src/filter/cbf/cbf.hpp"
#include "../src/filter/tbf/tbf.hpp"
#include "../src/filter/vbf/vbf.hpp"
#include "doctest/doctest/doctest.h"

#define numOfInput 6391378
#define numOfTestRepetition 5
#define numOfHashFuntions 5

// (insert, lookup, delete)
using BenchmarkArray =
    std::array<std::tuple<float, float, float>, numOfTestRepetition>;

using InputArray = std::array<std::vector<uint8_t>, numOfInput>;

std::string shuffleString(const std::string& input) {
    std::string shuffled = input;  // Create a copy of the original string

    // Seed for randomness
    std::random_device rd;
    std::mt19937 generator(rd());

    // Shuffle the characters in the string
    std::shuffle(shuffled.begin(), shuffled.end(), generator);

    return shuffled;
}

// template <typename T>
// void runTest(T& bf, size_t index, BenchmarkArray& benchmarkArr,
//              const InputArray& lines) {
//     size_t falsePositiveCount = 0;

//     // Benchmark add
//     auto startAdd = std::chrono::system_clock::now();
//     for (size_t i = 0; i < numOfInput; ++i) {
//         bf.add(lines[i].data(), lines[i].size());
//     }
//     auto endAdd = std::chrono::system_clock::now();
//     float addTime =
//         std::chrono::duration_cast<std::chrono::milliseconds>(endAdd -
//         startAdd)
//             .count();

//     // Benchmark Lookup
//     auto startLookup = std::chrono::system_clock::now();
//     for (size_t i = 0; i < numOfInput; ++i) {
//         // const uint8_t* key = reinterpret_cast<const
//         // uint8_t*>(lines[i].c_str()); uint16_t keyLength =
//         // static_cast<uint16_t>(lines[i].size()); CHECK_EQ(bf.lookup(key,
//         // keyLength), true);
//     }
//     auto endLookUp = std::chrono::system_clock::now();
//     float lookupTime = std::chrono::duration_cast<std::chrono::milliseconds>(
//                            endLookUp - startLookup)
//                            .count();

//     // False positive
//     for (size_t i = 0; i < numOfInput; ++i) {
//         // auto falseString = shuffleString(lines[i]);
//         // const uint8_t* key =
//         //     reinterpret_cast<const uint8_t*>(falseString.c_str());
//         // uint16_t keyLength = static_cast<uint16_t>(falseString.size());
//         // if (bf.lookup(key, keyLength)) {
//         //     ++falsePositiveCount;
//         // }
//     }

//     // Benchmark remove
//     auto startRemove = std::chrono::system_clock::now();
//     for (size_t i = 0; i < numOfInput; ++i) {
//         // const uint8_t* key = reinterpret_cast<const
//         // uint8_t*>(lines[i].c_str()); uint16_t keyLength =
//         // static_cast<uint16_t>(lines[i].size()); bf.remove(key, keyLength);
//     }

//     auto endRemove = std::chrono::system_clock::now();
//     float removeTime = std::chrono::duration_cast<std::chrono::milliseconds>(
//                            endRemove - startRemove)

//                            .count();

//     benchmarkArr[index] = std::make_tuple(addTime, lookupTime, removeTime);

//     printf("\nFalse Positive Rate: %f\n\n",
//            static_cast<float>(falsePositiveCount) * 100 / numOfInput);
// }

TEST_CASE("Benchmark Couting Bloom Filter") {
    CountingBloomFilter bf{numOfInput, numOfHashFuntions};
    BenchmarkArray benchmarkArr{};
    InputArray inputArr{};

    std::ifstream file("urls");
    if (file.is_open()) {
        for (std::string str; std::getline(file, str);) {
            inputArr[i] = std::vector<uint8_t>(str.begin(), str.end());
        }
        file.close();
    } else {
        std::cerr << "Unable to open file." << std::endl;
        exit(1);
    }

    // for (size_t i = 0; i < numOfTestRepetition; ++i) {
    //     runTest<CountingBloomFilter>(bf, i, benchmarkArr, inputArr);
    // }
}
