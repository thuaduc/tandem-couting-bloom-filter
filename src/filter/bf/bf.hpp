#pragma once
#include <math.h>

#include <array>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include "../../ultility/help.hpp"
#include "../../ultility/murmurHash.hpp"
#include "../../ultility/sha.hpp"

#define nHashFunctions 4
#define u8size 8

class BloomFilter {
   public:
    BloomFilter(size_t size);
    virtual bool add(std::string_view item);
    virtual bool lookup(std::string_view item);
    virtual ~BloomFilter() = default;
    size_t size();
    void printBits(uint8_t value);
    void print();

   protected:
    size_t _size;
    std::vector<uint8_t> _filter;
    std::unique_ptr<std::function<uint64_t(std::string_view)>[]> hashFunctions;

   private:
    // set bit at given index to 1
    // only for standard bloom filter
    void setBit(size_t index);

    // check if bit is set at given index
    // only for standard bloom filer
    bool isSet(size_t index);
};