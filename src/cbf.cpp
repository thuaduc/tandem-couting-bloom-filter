#include "cbf.hpp"
#include <iostream>

CountingBloomFilter::CountingBloomFilter(size_t m, uint8_t k) : filter(ceil(m/2.0)), f_set{murmurHash64A_Vector(k)}{}

void CountingBloomFilter::add(uint8_t *key, uint16_t keyLength){
    for (size_t i = 0; i < f_set.size(); ++i) {
        size_t index = f_set.at(i)(key, keyLength) % filter.size();
        incrementNibble(index);
    }
}

bool CountingBloomFilter::lookup(uint8_t *key, uint16_t keyLength){
    for (size_t i = 0; i < f_set.size(); ++i) {
        size_t index = f_set.at(i)(key, keyLength) % filter.size();
        if(!getNibble(index)){
            return false;
        }
    }
    return true;
}

bool CountingBloomFilter::remove(uint8_t *key, uint16_t keyLength){
    if(!lookup(key, keyLength)){
        return false;
    }

    for (size_t i = 0; i < f_set.size(); ++i) {
        size_t index = f_set.at(i)(key, keyLength) % filter.size();
        decrementNibble(index);
    }
    return true;
}

 void CountingBloomFilter::incrementNibble(size_t index){
    size_t pos = index / 2;
    uint8_t msNibble;
    uint8_t lsNibble;

    // increment low part of uint8
    if ((index & 1) == 0) {
        lsNibble = filter.at(pos) & LS_NIBBLE_MASK;
        if (lsNibble == 15) {
            std::cerr << "Counter overflow by couting bloom filter" << std::endl;
            return;
        }
        msNibble = filter.at(pos) & MS_NIBBLE_MASK;
        filter.at(pos) = msNibble | (lsNibble + 1);
    }
    // increment high part of uint8
    else {
        msNibble = (filter.at(pos) & MS_NIBBLE_MASK) >> 4;
        if (msNibble == 15) {
            std::cerr << "Counter overflow by couting bloom filter" << std::endl;
            return;
        }
        lsNibble = filter.at(pos) & LS_NIBBLE_MASK;
        filter.at(pos) = ((msNibble + 1) << 4) | lsNibble;
    }
 }

void CountingBloomFilter::decrementNibble(size_t index){
    size_t pos = index / 2;
    uint8_t msNibble;
    uint8_t lsNibble;

    // decrement low part of uint8
    if ((index & 1) == 0) {
        lsNibble = filter.at(pos) & LS_NIBBLE_MASK;
        if (lsNibble == 0) {
            std::cerr << "Counter underflow by couting bloom filter" << std::endl;
            return;
        }
        msNibble = filter.at(pos) & MS_NIBBLE_MASK;
        filter.at(pos) = msNibble | (lsNibble - 1);
    }
    // decrement high part of uint8
    else {
        msNibble = (filter.at(pos) & MS_NIBBLE_MASK) >> 4;
        if (msNibble == 0) {
            std::cerr << "Counter underflow by couting bloom filter" << std::endl;
            return;
        }
        lsNibble = filter.at(pos) & LS_NIBBLE_MASK;
        filter.at(pos) = ((msNibble - 1) << 4) | lsNibble;
    }
}

uint8_t CountingBloomFilter::getNibble(size_t index){
    size_t pos = index / 2;
    return (index & 1) == 0 ? filter.at(pos) & LS_NIBBLE_MASK : (filter.at(pos) & MS_NIBBLE_MASK) >> 4;
}
