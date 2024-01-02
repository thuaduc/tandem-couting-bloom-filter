#include <iostream>

#include "filter/bf/bf.hpp"
#include "ultility/sha.hpp"

int main() {
    std::string input = "Hello, SHA-256!";
    std::string input2 = "Hello, SHA-259!";

    std::string hash_result = SHA::sha256(input);

    std::cout << "Input: " << input << std::endl;
    std::cout << "SHA-256 Hash: " << hash_result << std::endl;

    std::array<std::size_t, 5> result = SHA::createHashArray(input);

    for (const auto& hash : result) {
        std::cout << hash << std::endl;
    }

    constexpr size_t items = 1000000;
    constexpr size_t filterSize = static_cast<size_t>(8.127152913 * items);
    BloomFilter<filterSize> filter;

    std::cout << filter.size() << std::endl;
    std::cout << filter.realSizeInMB() << std::endl;
    std::cout << filter.add(input) << std::endl;
    std::cout << filter.search(input) << std::endl;
    std::cout << filter.search(input2) << std::endl;

    return 0;
}