#include "bf.hpp"

BloomFilter::BloomFilter(size_t m, uint8_t k) : filter(ceil(m/8.0)), f_set{murmurHash64A_Vector(k)}{}

void BloomFilter::add(uint8_t* key, uint16_t keyLength) {
    for (size_t i = 0; i < f_set.size(); ++i) {
        size_t index = f_set.at(i)(key, keyLength) % filter.size();
        setBit(index);
    }
}

bool BloomFilter::lookup(uint8_t* key, uint16_t keyLength) {
    for (size_t i = 0; i < f_set.size(); ++i) {
        size_t index = f_set.at(i)(key, keyLength) % filter.size();
        if (!getBit(index)) {
            return false;
        }
    }
    return true;
}

void BloomFilter::setBit(size_t index) {
    size_t pos = index / 8;
    uint8_t mask = 1 << (index - (pos * 8));
    filter.at(pos) = filter.at(pos) | mask;
}

uint8_t BloomFilter::getBit(size_t index) {
    size_t pos = index / 8;
    uint8_t mask = 1 << (index - (pos * 8));
    return filter.at(pos) & mask;
}