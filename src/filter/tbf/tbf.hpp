#pragma once
#include "murmurHash.hpp"
#include <iostream>
#include <cstdint>
#include <vector>
#include <tuple>

class TandemBloomFilter {
   public:
    TandemBloomFilter(size_t size, uint8_t nHashFunctions, uint8_t L_set);
    void add(uint8_t *key, uint16_t keyLength);
    bool lookup(uint8_t *key, uint16_t keyLength);
    bool remove(uint8_t *key, uint16_t keyLength);

   private:
    std::vector<uint8_t> filter;
    std::vector<std::function<uint64_t(uint8_t* key, uint16_t keyLength)>> f_set;
    std::vector<std::function<uint64_t(uint8_t* key, uint16_t keyLength)>> g_set;
    std::vector<std::function<uint64_t(uint8_t* key, uint16_t keyLength)>> h_set;
    uint8_t L_set;
    void increment(size_t index, uint8_t var_increment);
    void decrement(size_t index, uint8_t var_increment);
    uint8_t getAdjecentIndex(size_t index);
    std::tuple<size_t, uint8_t, uint8_t> getTandemValues(uint8_t i, uint8_t *key, uint16_t keyLength);
};