#include <iostream>

#include "filter/bf/bf.hpp"
#include "ultility/sha.hpp"

int main() {
    std::string input = "Hello, SHA-256!";
    std::string hash_result = SHA::sha256(input);

    std::cout << "Input: " << input << std::endl;
    std::cout << "SHA-256 Hash: " << hash_result << std::endl;

    std::array<std::string, 5> result = SHA::createHashArray(input);

    for (const auto& hash : result) {
        std::cout << hash << std::endl;
    }
    // constexpr size_t len = HELP::calculateFilterLength(8000000);

    BloomFilter<1000000> filter;
    std::cout << filter.size() << std::endl;
    std::cout << filter.realSizeInMB() << std::endl;

    return 0;
}