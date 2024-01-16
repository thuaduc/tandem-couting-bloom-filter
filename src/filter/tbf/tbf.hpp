#pragma once
#include "../vbf/vbf.hpp"

class TandemBloomFilter : public VariableCoutingBloomFilter {
   public:
    TandemBloomFilter(size_t size);
    virtual bool add(std::string_view item) override;
    virtual bool lookup(std::string_view item) override;
    virtual bool remove(std::string_view item) override;

   private:
    std::tuple<size_t, size_t, uint8_t, uint8_t> calculatePositionAndHashes(
        size_t i, std::string_view item);
    bool increaseCounterNeighbour(size_t index, uint8_t num);
};