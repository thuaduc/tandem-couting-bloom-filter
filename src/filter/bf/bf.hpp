#pragma once
#include <cmath>

#include "../../ultility/help.hpp"
#include "../../ultility/sha.hpp"
#include "../../ultility/murmurHash.hpp"
#include <vector>
#include <array>
#include <concepts>
#include <iostream>
#include <memory>
#include <math.h>

#define ERRORRATE 0.02
#define nHashFunctions (floor(-log2(ERRORRATE))) //approximate value assuming 1 byte per item/element

template<std::integral T = bool>
class BloomFilter{
   public:
        BloomFilter(size_t size);
        virtual bool add(std::string_view item);
        virtual bool lookup(std::string_view item);
        virtual ~BloomFilter() = default;

   protected:
        size_t _size;
        std::vector<T> _filter;
        std::unique_ptr<std::function<uint64_t(uint64_t)>[]> hashFunctions;
};

template<std::integral T>
BloomFilter<T>::BloomFilter(size_t size) : _size{size}{
    _filter.resize(_size, 0);
    hashFunctions = murmurHash64A_Array(nHashFunctions);
}

template<std::integral T>
bool BloomFilter<T>::add(std::string_view item){
    auto hashArray = SHA::createHashArray(static_cast<std::string>(item));
    for (size_t i = 0; i < hashArray.size(); ++i) {
        auto pos = hashArray.at(i) % this->_size;
        if(this->_filter.at(pos) == 0){
            this->_filter.at(pos) = 1;
        }
    }
    return true;
}

template<std::integral T>
bool BloomFilter<T>::lookup(std::string_view item){
    for (size_t i = 0; i < 5; ++i) {
        auto pos =  hashFunctions[i](item) % this->_size;
        if(this->_filter.at(pos) == 0){
            return false;
        }
    }
    return true;
}