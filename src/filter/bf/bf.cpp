#include "bf.hpp"

BloomFilter::BloomFilter(size_t size) : _size{size * 8} {
    hashFunctions = murmurHash64A_Array(nHashFunctions);
    _filter.assign(size, 0);
}

size_t BloomFilter::size() { return _size; }

bool BloomFilter::add(std::string_view item) {
    for (size_t i = 0; i < nHashFunctions; ++i) {
        auto pos = hashFunctions[i](item) % this->_size;
        setBit(pos);
    }
    return true;
}

bool BloomFilter::lookup(std::string_view item) {
    for (size_t i = 0; i < nHashFunctions; ++i) {
        auto pos = hashFunctions[i](item) % this->_size;
        if (!isSet(pos)) {
            return false;
        }
    }
    return true;
}

void BloomFilter::setBit(size_t index) {
    size_t pos = index / u8size;
    uint8_t mask = 1 << (index - (pos * u8size));
    _filter.at(pos) = _filter.at(pos) | mask;
}

bool BloomFilter::isSet(size_t index) {
    size_t pos = index / u8size;
    uint8_t mask = 1 << (index - (pos * u8size));
    return (_filter.at(pos) & mask) != 0;
}

void BloomFilter::printBits(uint8_t value) {
    for (int i = 7; i >= 0; --i) {
        uint8_t bit = (value & (1 << i)) ? 1 : 0;
        printf("%hhu", bit);
    }
}

void BloomFilter::print() {
    for (const auto& value : _filter) {
        printBits(value);
        printf(" ");
    }
    printf("\n");
}