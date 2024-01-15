#pragma once
#include "../bf/bf.hpp"
#define L  // value increased in range L - 2L

class VariableCoutingBloomFilter : public BloomFilter {
   public:
    VariableCoutingBloomFilter(size_t size);
    virtual bool add(std::string_view item) override;
    virtual bool lookup(std::string_view item) override;
    virtual bool remove(std::string_view item);

   private:
    void increaseCounter(size_t index, uint8_t num);
    bool decreaseCounter(size_t index, uint8_t num);
    std::pair<size_t, uint8_t> calculatePositionAndHashValue(size_t hash);
};