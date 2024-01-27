#pragma once
#include <cstdint>
#include <vector>

#include "murmurHash.hpp"
#include "utility.hpp"

#define LS_NIBBLE_MASK 0x0f
#define MS_NIBBLE_MASK 0xf0

class CountingBloomFilter {
   public:
    CountingBloomFilter(size_t m, uint8_t k);
    void insert(uint8_t *key, uint16_t keyLength);
    bool lookup(uint8_t *key, uint16_t keyLength);
    bool remove(uint8_t *key, uint16_t keyLength);

   private:
    std::vector<std::function<uint64_t(uint8_t *key, uint16_t keyLength)>> f_set;
    std::vector<uint8_t> filter;
    uint64_t slots;
    
    void incrementNibble(size_t index);
    void decrementNibble(size_t index);
    uint8_t getNibble(size_t index);
};