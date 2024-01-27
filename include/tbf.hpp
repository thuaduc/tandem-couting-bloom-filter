#pragma once
#include <cstdint>
#include <vector>
#include <tuple>

#include "murmurHash.hpp"

class TandemBloomFilter {
   public:
    TandemBloomFilter(size_t m, uint8_t k, uint8_t L_set);
    void insert(uint8_t *key, uint16_t keyLength);
    bool lookup(uint8_t *key, uint16_t keyLength);
    bool remove(uint8_t *key, uint16_t keyLength);

   private:
    size_t m; 
    uint8_t k;
    std::vector<uint8_t> filter;
    std::vector<std::function<uint64_t(uint8_t *key, uint16_t keyLength)>> f_set;
    std::vector<std::function<uint64_t(uint8_t *key, uint16_t keyLength)>> g_set;
    std::vector<std::function<uint64_t(uint8_t *key, uint16_t keyLength)>> h_set;
    uint8_t L_set;
    void increment(uint8_t& toInc, uint8_t varIncrement);
    void decrement(uint8_t& toInc, uint8_t varIncrement);
    void decrement(size_t index, uint8_t var_increment);
    size_t getAdjecentIndex(size_t index);
    std::tuple<size_t, uint8_t, uint8_t> getTBFvalues(uint8_t i, uint8_t *key, uint16_t keyLength);
};