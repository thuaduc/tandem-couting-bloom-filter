#include "bf.hpp"

BloomFilter::BloomFilter(size_t size) : _size{size}{
    _filter.resize(_size, 0);
    hashFunctions = murmurHash64A_Array(nHashFunctions);
}

bool BloomFilter::add(std::string_view item){
    for (size_t i = 0; i < nHashFunctions; ++i) {
        auto pos = hashFunctions[i](item) % this->_size;
        if(this->_filter.at(pos) == 0){
            this->_filter.at(pos) = 1;
        }
    }
    return true;
}

bool BloomFilter::lookup(std::string_view item){
    for (size_t i = 0; i < nHashFunctions; ++i) {
        auto pos = hashFunctions[i](item) % this->_size;
        if(this->_filter.at(pos) == 0){
            return false;
        }
    }
    return true;
}