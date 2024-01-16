#include <array>
#include <cstdint>
#include <iostream>

#include "filter/bf/bf.hpp"
#include "filter/cbf/cbf.hpp"
#include "filter/tbf/tbf.hpp"
#include "filter/vbf/vbf.hpp"
#include "ultility/murmurHash.hpp"
#include "ultility/sha.hpp"

int main() {
    size_t size = 1000;
    std::string someRandomText = "Some random text";

    BloomFilter bf{size};
    bf.add(someRandomText);
    bf.lookup(someRandomText);
    std::cout << bf.size() << std::endl;

    CountingBloomFilter cbf{size};
    std::cout << cbf.size() << std::endl;
    cbf.add(someRandomText);
    cbf.lookup(someRandomText);
    cbf.remove(someRandomText);

    VariableCoutingBloomFilter vbf{size};
    vbf.add(someRandomText);
    vbf.lookup(someRandomText);
    vbf.remove(someRandomText);

    TandemBloomFilter tbf{size};
    tbf.add(someRandomText);
    tbf.lookup(someRandomText);
    tbf.remove(someRandomText);
}