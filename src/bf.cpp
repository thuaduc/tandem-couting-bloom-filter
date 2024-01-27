#include "bf.hpp"
 
BloomFilter::BloomFilter(size_t m, uint8_t k): 
    f_set{setOfMurmurHash64A(k)},
    filter(roundUp8(m) >> 3),
    slots{m}{}

void BloomFilter::insert(uint8_t* key, uint16_t keyLength) {
    for (uint8_t i = 0; i < f_set.size(); ++i) {
        size_t index = f_set.at(i)(key, keyLength) % slots;
        setBit(index);
    }
}

bool BloomFilter::lookup(uint8_t* key, uint16_t keyLength) {
    for (uint8_t i = 0; i < f_set.size(); ++i) {
        size_t index = f_set.at(i)(key, keyLength) % slots;
        if (!getBit(index)) {
            return false;
        }
    }
    return true;
}

void BloomFilter::setBit(size_t index) {
    size_t pos = index >> 3;
    uint8_t mask = 0b10000000 >> (index - (pos << 3));
    filter.at(pos) = filter.at(pos) | mask;
}

uint8_t BloomFilter::getBit(size_t index) {
    size_t pos = index >> 3;
    uint8_t mask = 0b10000000 >> (index - (pos << 3));
    return filter.at(pos) & mask;
}