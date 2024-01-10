#pragma once
#include <math.h>

#include <array>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include "../../ultility/array.hpp"
#include "../../ultility/help.hpp"
#include "../../ultility/murmurHash.hpp"
#include "../../ultility/sha.hpp"

// #define ERRORRATE 0.02
#define nHashFunctions 5

class BloomFilter {
   public:
    BloomFilter(size_t size);
    virtual bool add(std::string_view item);
    virtual bool lookup(std::string_view item);
    virtual ~BloomFilter() = default;

   protected:
    size_t _size;
    std::vector<uint8_t> _filter;
    std::unique_ptr<std::function<uint64_t(std::string_view)>[]> hashFunctions;
};