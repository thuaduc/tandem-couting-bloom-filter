#include "cbf.hpp"

CountingBloomFilter::CountingBloomFilter(size_t size) : BloomFilter(size) {}

bool CountingBloomFilter::add(std::string_view item) {
    for (size_t i = 0; i < nHashFunctions; ++i) {
        auto pos = hashFunctions[i](item) % this->_size;
    }
    return true;
}

bool CountingBloomFilter::remove(std::string_view item) {
    if (!lookup(item)) {
        return false;
    }

    for (size_t i = 0; i < nHashFunctions; ++i) {
        auto pos = hashFunctions[i](item) % this->_size;
    }
    return true;
}
