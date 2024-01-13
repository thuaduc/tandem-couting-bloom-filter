#include <iostream>

#include "filter/bf/bf.hpp"
#include "filter/cbf/cbf.hpp"
#include "ultility/sha.hpp"
#include <array>
#include <cstdint>
#include "ultility/murmurHash.hpp"

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

    constexpr size_t items = 1e3;
    constexpr size_t filterSize = static_cast<size_t>(8.127152913 * items);
    BloomFilter bf{filterSize};

    std::string_view v{"Test"};
    bf.add(v);

    CountingBloomFilter<long> cbf{filterSize};
    std::cout << cbf.lookup(v) << std::endl;
    cbf.add(v);
    std::cout << cbf.lookup(v) << std::endl;

    auto x = murmurHash64A_Array(3);
    auto f1 = x[0];
    auto f2 = x[1];
    auto f3 = x[2];

    return 0;
}