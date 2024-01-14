#pragma once
#include "../bf/bf.hpp"

class CountingBloomFilter : public BloomFilter {
   public:
    CountingBloomFilter(size_t size);
    virtual bool add(std::string_view item) override;
    virtual bool lookup(std::string_view item) override;
    virtual bool remove(std::string_view item);

   private:
    void incrementCounterByOne(size_t index);
    bool decrementCounterByOne(size_t index);
    bool isCounterSet(size_t index);
    void checkIndexValid(size_t index);
};