#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../src/filter/bf/bf.hpp"
#include "../src/filter/cbf/cbf.hpp"
#include "../src/filter/tbf/tbf.hpp"
#include "../src/filter/vbf/vbf.hpp"
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

    printf("\nFalse Positive Rate: %f\n\n",
           static_cast<float>(falsePositiveCount) / size);
}

TEST_CASE("Test basic insert, remove, lookup - couting bloom filter") {
    size_t size = 1000000;
    std::string someRandomText = "Some random text";
    CountingBloomFilter cbf{size};
    size_t falsePositiveCount = 0;

    for (size_t i = 0; i < size; i += 2) {
        cbf.add(someRandomText + std::to_string(i));
    }

    for (size_t i = 0; i < size; i++) {
        if (i % 2 == 0) {
            CHECK_EQ(cbf.lookup(someRandomText + std::to_string(i)), true);
        } else {
            if (cbf.lookup(someRandomText + std::to_string(i))) {
                falsePositiveCount++;
            }
        }
    }

    for (size_t i = 0; i < size; i += 2) {
        if (i % 2 == 0) {
            CHECK_EQ(cbf.remove(someRandomText + std::to_string(i)), true);
        }
    }

    for (size_t i = 0; i < size; i += 2) {
        CHECK_EQ(cbf.lookup(someRandomText + std::to_string(i)), false);
    }

    printf("\nFalse Positive Rate: %f\n\n",
           static_cast<float>(falsePositiveCount) / size);
}

TEST_CASE(
    "Test basic insert, remove, lookup - variable increment bloom filter") {
    size_t size = 1000000;
    std::string someRandomText = "Some random text";
    VariableCoutingBloomFilter vbf{size};
    size_t falsePositiveCount = 0;

    for (size_t i = 0; i < size; i += 2) {
        vbf.add(someRandomText + std::to_string(i));
    }

    for (size_t i = 0; i < size; i++) {
        if (i % 2 == 0) {
            CHECK_EQ(vbf.lookup(someRandomText + std::to_string(i)), true);
        } else {
            if (vbf.lookup(someRandomText + std::to_string(i))) {
                falsePositiveCount++;
            }
        }
    }

    for (size_t i = 0; i < size; i += 2) {
        if (i % 2 == 0) {
            CHECK_EQ(vbf.remove(someRandomText + std::to_string(i)), true);
        }
    }

    for (size_t i = 0; i < size; i += 2) {
        CHECK_EQ(vbf.lookup(someRandomText + std::to_string(i)), false);
    }

    printf("\nFalse Positive Rate: %f\n\n",
           static_cast<float>(falsePositiveCount) / size);
}

// TEST_CASE("Test basic insert, remove, lookup - tandem bloom filter") {
//     size_t size = 1000000;
//     std::string someRandomText = "Some random text";
//     TandemBloomFilter tbf{size};
//     size_t falsePositiveCount = 0;

//     for (size_t i = 0; i < size; i += 2) {
//         tbf.add(someRandomText + std::to_string(i));
//     }

//     for (size_t i = 0; i < size; i++) {
//         if (i % 2 == 0) {
//             CHECK_EQ(tbf.lookup(someRandomText + std::to_string(i)), true);
//         } else {
//             if (tbf.lookup(someRandomText + std::to_string(i))) {
//                 falsePositiveCount++;
//             }
//         }
//     }

//     for (size_t i = 0; i < size; i += 2) {
//         if (i % 2 == 0) {
//             CHECK_EQ(tbf.remove(someRandomText + std::to_string(i)), true);
//         }
//     }

//     for (size_t i = 0; i < size; i += 2) {
//         CHECK_EQ(tbf.lookup(someRandomText + std::to_string(i)), false);
//     }

//     printf("\nFalse Positive Rate: %f\n\n",
//            static_cast<float>(falsePositiveCount) / size);
// }