#pragma once
#include <cstdint>
#include <vector>
#include <cmath>

#include "murmurHash.hpp"

class BloomFilter {
   public:
    BloomFilter(size_t m, uint8_t k);
    void insert(uint8_t *key, uint16_t keyLength);
    bool lookup(uint8_t *key, uint16_t keyLength);
    ~BloomFilter() = default;

   private:
    size_t m; 
    uint8_t k;
    std::vector<uint8_t> filter;
    std::vector<std::function<uint64_t(uint8_t* key, uint16_t keyLength)>> f_set;
    void setBit(size_t index);
    uint8_t getBit(size_t index);
};