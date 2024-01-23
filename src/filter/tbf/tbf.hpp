#pragma once
#include <cstdint>
#include <iostream>
#include <tuple>
#include <vector>

#include "../../ultility/murmurHash.hpp"

class TandemBloomFilter {
   public:
    TandemBloomFilter(size_t size, uint8_t nHashFunctions, uint8_t L_set);
    void add(uint8_t* key, uint16_t keyLength);
    bool lookup(uint8_t* key, uint16_t keyLength);
    bool remove(uint8_t* key, uint16_t keyLength);

   private:
    std::vector<uint8_t> filter;
    std::vector<std::function<uint64_t(uint8_t* key, uint16_t keyLength)>>
        f_set;
    std::vector<std::function<uint64_t(uint8_t* key, uint16_t keyLength)>>
        g_set;
    std::vector<std::function<uint64_t(uint8_t* key, uint16_t keyLength)>>
        h_set;
    uint8_t L_set;
    void increment(uint8_t& toInc, uint8_t varIncrement);
    void decrement(uint8_t& toInc, uint8_t varIncrement);
    void decrement(size_t index, uint8_t var_increment);
    uint8_t getAdjecentIndex(size_t index);
    std::tuple<size_t, uint8_t, uint8_t> getTBFvalues(uint8_t i, uint8_t* key,
                                                      uint16_t keyLength);
};