#pragma once
#include <bitset>
#include <cmath>

#include "../../ultility/help.hpp"
#include "../../ultility/sha.hpp"

template <size_t T>
class BloomFilter {
   public:
    BloomFilter();
    size_t size();
    std::string realSizeInMB();
    bool add(const std::string& item);
    bool search(const std::string& item);

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

template <size_t T>
bool BloomFilter<T>::add(const std::string& item) {
    auto hashArray = SHA::createHashArray(item);
    for (size_t i = 0; i < 5; i++) {
        _filter[hashArray.at(i) % T] = 1;
    }
    return true;
}

template <size_t T>
bool BloomFilter<T>::search(const std::string& item) {
    auto hashArray = SHA::createHashArray(item);
    for (size_t i = 0; i < 5; i++) {
        if (_filter[hashArray.at(i) % T] == 0) return false;
    }
    return true;
}
