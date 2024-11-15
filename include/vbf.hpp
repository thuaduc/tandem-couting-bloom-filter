#pragma once
#include <cstdint>
#include <vector>
#include <utility>

#include "murmurHash.hpp"
#include "utility.hpp"
class VariableCoutingBloomFilter
{
public:
    VariableCoutingBloomFilter(uint64_t m, uint8_t k, uint8_t L_set);
    void insert(uint8_t *key, uint16_t keyLength);
    bool lookup(uint8_t *key, uint16_t keyLength);
    bool remove(uint8_t *key, uint16_t keyLength);

private:
    std::vector<std::function<uint64_t(uint8_t *key, uint16_t keyLength)>> f_set;
    std::vector<std::function<uint64_t(uint8_t *key, uint16_t keyLength)>> g_set;
    std::vector<uint8_t> filter;
    uint8_t L_set;

    void increment(size_t index, uint8_t var_increment);
    void decrement(size_t index, uint8_t var_increment);
    std::pair<size_t, uint8_t> getVBFvalues(size_t i, uint8_t *key, uint16_t keyLength);
};
