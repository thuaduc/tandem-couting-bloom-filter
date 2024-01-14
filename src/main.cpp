#include <array>
#include <cstdint>
#include <iostream>

#include "filter/bf/bf.hpp"
#include "filter/cbf/cbf.hpp"
#include "ultility/murmurHash.hpp"
#include "ultility/sha.hpp"

int main() {
    BloomFilter bf{16};
    // should be 2
    std::cout << bf.size() << std::endl;

    bf.add("item1");
    bf.add("item2");
    // should be true true false
    std::cout << bf.lookup("item1") << " " << bf.lookup("item2") << " "
              << bf.lookup("item3") << std::endl;

    bf.print();

    CountingBloomFilter cbf{1024};
    // should be 8
    std::cout << cbf.size() << std::endl;
    for (size_t i = 0; i < 1023; i++) {
        cbf.add("item" + std::to_string(i));
    }

    std::cout << cbf.lookup("item1") << " " << cbf.lookup("item2") << " "
              << cbf.lookup("item3") << std::endl;

    cbf.print();
}