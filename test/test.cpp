#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <random>
#include <string>

#include "../src/filter/bf/bf.hpp"
#include "../src/filter/cbf/cbf.hpp"
#include "../src/filter/tbf/tbf.hpp"
#include "../src/filter/vbf/vbf.hpp"
#include "doctest/doctest/doctest.h"

size_t size = 10000000;
size_t size2 = 10000000;
std::string someRandomText = "This is a random text";

std::string generateRandomString(std::size_t length) {
    static const char charset[] =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static const std::size_t charsetSize =
        sizeof(charset) - 1;  // excluding null terminator

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> dis(0, charsetSize - 1);

    std::string randomString;
    randomString.reserve(length);

    for (std::size_t i = 0; i < length; ++i) {
        randomString.push_back(charset[dis(gen)]);
    }

    return randomString;
}

void runTest(BloomFilter& bf) {
    size_t falsePositiveCount = 0;
    std::string typeOfFilter = "";

    if (typeid(bf) == typeid(CountingBloomFilter)) {
        typeOfFilter = "Counting Bloomfilter";
    } else if (typeid(bf) == typeid(VariableCoutingBloomFilter)) {
        typeOfFilter = "Variable Conting Bloomfilter";
    } else if (typeid(bf) == typeid(TandemBloomFilter)) {
        typeOfFilter = "Tandem Bloomfilter";
    } else {
        typeOfFilter = "Bloomfilter";
    }

    auto start = std::chrono::system_clock::now();
    for (size_t i = 0; i < size; ++i) {
        bf.add(someRandomText + std::to_string(i));
    }
    auto end = std::chrono::system_clock::now();
    float elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();
    printf("Insert Time %s %.4fms\n", typeOfFilter.c_str(), elapsed);

    start = std::chrono::system_clock::now();
    for (size_t i = 0; i < size; ++i) {
        CHECK_EQ(bf.lookup(someRandomText + std::to_string(i)), true);
    }
    end = std::chrono::system_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                  .count();
    printf("Lookup Time %s %.4fms", typeOfFilter.c_str(), elapsed);

    for (size_t i = size; i < size + size2; ++i) {
        if (bf.lookup(someRandomText + std::to_string(i))) {
            ++falsePositiveCount;
        }
    }

    // if (typeOfFilter.c_str() != "Bloomfilter") {
    //     start = std::chrono::system_clock::now();
    //     for (size_t i = size; i < size + size2; ++i) {
    //         bf.remove(someRandomText + std::to_string(i));
    //     }
    //     end = std::chrono::system_clock::now();
    //     elapsed =
    //         std::chrono::duration_cast<std::chrono::milliseconds>(end -
    //         start)
    //             .count();
    //     printf("Remove Time %.4fms\n", elapsed);
    // }

    printf("\nFalse Positive Rate %s: %f\n\n", typeOfFilter.c_str(),
           static_cast<float>(falsePositiveCount) * 100 / size);
}

TEST_CASE("Test basic insert, remove, lookup - standart bloom filter") {
    BloomFilter bf{size};
    runTest(bf);
}

TEST_CASE("Test basic insert, remove, lookup - couting bloom filter") {
    CountingBloomFilter bf{size};
    runTest(bf);
}

TEST_CASE("Test basic insert, remove, lookup - variable couting bloom filter") {
    VariableCoutingBloomFilter bf{size};
    runTest(bf);
}

TEST_CASE("Test basic insert, remove, lookup - tandem couting bloom filter") {
    TandemBloomFilter bf{size};
    runTest(bf);
}
