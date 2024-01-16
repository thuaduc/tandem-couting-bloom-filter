#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../src/filter/bf/bf.hpp"
#include "../src/filter/cbf/cbf.hpp"
#include "../src/filter/tbf/tbf.hpp"
#include "../src/filter/vbf/vbf.hpp"
#include "doctest/doctest/doctest.h"

size_t size = 1000000;
size_t size2 = 100000;
std::string someRandomText = "This is a random text";

void runTest(BloomFilter& bf) {
    size_t falsePositiveCount = 0;

    for (size_t i = 0; i < size; ++i) {
        bf.add(someRandomText + std::to_string(i));
    }

    for (size_t i = 0; i < size; ++i) {
        CHECK_EQ(bf.lookup(someRandomText + std::to_string(i)), true);
    }

    for (size_t i = size; i < size + size2; ++i) {
        if (bf.lookup(someRandomText + std::to_string(i))) {
            ++falsePositiveCount;
        }
    }

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

    printf("\nFalse Positive Rate %s: %f\n\n", typeOfFilter.c_str(),
           static_cast<float>(falsePositiveCount) / size);
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
