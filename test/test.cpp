#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../src/filter/bf/bf.hpp"
#include "doctest/doctest/doctest.h"

TEST_CASE("Test basic insert, remove, lookup - standart bloom filter") {
    size_t size = 1000000;
    std::string someRandomText = "Some random text";
    BloomFilter bf{size};
    size_t falsePositiveCount = 0;

    for (size_t i = 0; i < size; i += 2) {
        bf.add(someRandomText + std::to_string(i));
    }

    for (size_t i = 0; i < size; i++) {
        if (i % 2 == 0) {
            CHECK_EQ(bf.lookup(someRandomText + std::to_string(i)), true);
        } else {
            if (bf.lookup(someRandomText + std::to_string(i))) {
                falsePositiveCount++;
            }
        }
    }

    printf("False Positive Rate: %f\n",
           static_cast<float>(falsePositiveCount) / size);
}