#pragma once
#include <cmath>
#include <cstdint>
#include <vector>

#include "../../ultility/murmurHash.hpp"

#define MS_NIBBLE_MASK 0xf0
#define LS_NIBBLE_MASK 0x0f

class CountingBloomFilter {
   public:
    CountingBloomFilter(size_t size, uint8_t nHashFunctions);
    void add(const uint8_t *key, uint16_t keyLength);
    bool lookup(const uint8_t *key, uint16_t keyLength);
    bool remove(const uint8_t *key, uint16_t keyLength);
    ~CountingBloomFilter() = default;

   private:
    std::vector<uint8_t> filter;
    std::vector<std::function<uint64_t(const uint8_t *key, uint16_t keyLength)>>
        f_set;
    void incrementNibble(size_t index);
    void decrementNibble(size_t index);
    uint8_t getNibble(size_t index);
};