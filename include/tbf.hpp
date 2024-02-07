#pragma once
#include <cstdint>
#include <vector>
#include <tuple>

#include "murmurHash.hpp"
#include "utility.hpp"

class TandemBloomFilter
{
public:
    TandemBloomFilter(uint64_t m, uint8_t k, uint8_t L_set);
    void insert(uint8_t *key, uint16_t keyLength);
    bool lookup(uint8_t *key, uint16_t keyLength);
    bool remove(uint8_t *key, uint16_t keyLength);

private:
    std::vector<std::function<uint64_t(uint8_t *key, uint16_t keyLength)>> f_set;
    std::vector<std::function<uint64_t(uint8_t *key, uint16_t keyLength)>> g_set;
    std::vector<std::function<uint64_t(uint8_t *key, uint16_t keyLength)>> h_set;
    std::vector<uint8_t> filter;
    uint8_t L_set;

    void increment(uint8_t &toInc, uint8_t varIncrement);
    void decrement(uint8_t &toDec, uint8_t varIncrement);
    size_t getAdjacentIndex(size_t index);
    std::tuple<size_t, uint8_t, uint8_t> getTBFvalues(uint8_t i, uint8_t *key, uint16_t keyLength);
};