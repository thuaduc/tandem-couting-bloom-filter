#pragma once
#include <cmath>
#include <cstdint>
#include <vector>

#include "../../ultility/murmurHash.hpp"

class BloomFilter {
   public:
    BloomFilter(size_t size, uint8_t nHashFunctions);
    void add(uint8_t *key, uint16_t keyLength);
    bool lookup(uint8_t *key, uint16_t keyLength);
    ~BloomFilter() = default;

   private:
    std::vector<uint8_t> filter;
    std::vector<std::function<uint64_t(const uint8_t *key, uint16_t keyLength)>>
        f_set;
    void setBit(size_t index);
    uint8_t getBit(size_t index);
};