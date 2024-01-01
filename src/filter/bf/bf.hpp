#pragma once
#include <bitset>
#include <cmath>

#include "../../ultility/help.hpp"

template <size_t T>
class BloomFilter {
   public:
    BloomFilter();
    size_t size();
    std::string realSizeInMB();

   private:
    size_t _size = T;
    std::bitset<T> _filter;
};

template <size_t T>
BloomFilter<T>::BloomFilter() {
    // Constructor implementation if needed
}

template <size_t T>
size_t BloomFilter<T>::size() {
    return _size;
}

template <size_t T>
std::string BloomFilter<T>::realSizeInMB() {
    float sizeInMB = static_cast<float>(sizeof(_filter)) / 1024 / 1024;
    return std::to_string(sizeInMB) + " MB";
}
