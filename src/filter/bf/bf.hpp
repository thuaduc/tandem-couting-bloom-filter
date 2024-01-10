#pragma once
#include <cmath>

#include "../../ultility/help.hpp"
#include "../../ultility/sha.hpp"
#include <vector>
#include <array>
#include <concepts>
#include <iostream>

template<std::integral T = bool>
class BloomFilter{
   public:
    BloomFilter(size_t size);
    virtual bool add(std::string_view item);
    virtual bool lookup(std::string_view item);
    virtual ~BloomFilter() = default;

   private:
    size_t _size;
    std::vector<T> _filter;
};

template<std::integral T>
BloomFilter<T>::BloomFilter(size_t size) : _size{size}{
    _filter.resize(_size, 0);
}

template<std::integral T>
bool BloomFilter<T>::add(std::string_view item){
    auto hashArray = SHA::createHashArray(static_cast<std::string>(item));
    for (size_t i = 0; i < hashArray.size(); ++i) {
        auto pos = hashArray.at(i) % _size;
        if(_filter.at(pos) == 0){
            _filter.at(pos) = 1;
        }
    }
    return true;
}

template<std::integral T>
bool BloomFilter<T>::lookup(std::string_view item){
    auto hashArray = SHA::createHashArray(static_cast<std::string>(item));
    for (size_t i = 0; i < hashArray.size(); ++i) {
        auto pos = hashArray.at(i) % _size;
        if(_filter.at(pos) == 0){
            return false;
        }
    }
    return true;
}
