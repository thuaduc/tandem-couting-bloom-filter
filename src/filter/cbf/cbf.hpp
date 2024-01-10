#pragma once
#include "../bf/bf.hpp"

template<std::integral T = long>
class CountingBloomFilter : public BloomFilter<T>
{
    public:
        CountingBloomFilter(size_t size);
        virtual bool add(std::string_view item) override;
        virtual bool lookup(std::string_view item) override;
        virtual bool remove(std::string_view item);

};

template<std::integral T>
CountingBloomFilter<T>::CountingBloomFilter(size_t size): BloomFilter<T>(size) {}

template<std::integral T>
bool CountingBloomFilter<T>::add(std::string_view item) {
    auto hashArray = SHA::createHashArray(static_cast<std::string>(item));
    for (size_t i = 0; i < hashArray.size(); ++i) {
        auto pos = hashArray.at(i) % this->_size;
        ++this->_filter.at(pos);
    }
    return true;
}

template<std::integral T>
bool CountingBloomFilter<T>::lookup(std::string_view item) {
    auto hashArray = SHA::createHashArray(static_cast<std::string>(item));
    for (size_t i = 0; i < hashArray.size(); ++i) {
        auto pos = hashArray.at(i) % this->_size;
        if(this->_filter.at(pos) == 0){
            return false;
        }
    }
    return true;
}

template<std::integral T>
bool CountingBloomFilter<T>::remove(std::string_view item){
    if(!lookup(item)){
        return false;
    }

    auto hashArray = SHA::createHashArray(static_cast<std::string>(item));
    for (size_t i = 0; i < hashArray.size(); ++i) {
        auto pos = hashArray.at(i) % this->_size;
        --this->_filter.at(pos);
    }
    return true;
}



