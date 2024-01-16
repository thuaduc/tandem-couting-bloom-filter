#pragma once
#include "../vbf/vbf.hpp"

class TandemBloomFilter : public VariableCoutingBloomFilter {
   public:
    TandemBloomFilter(size_t size);
    virtual bool add(std::string_view item) override;
    virtual bool lookup(std::string_view item) override;
    virtual bool remove(std::string_view item) override;

   private:
    std::unique_ptr<std::function<uint64_t(std::string_view)>[]>
        thirdHashFunction;
    std::tuple<size_t, size_t, uint8_t, uint8_t>
    calculatePositionAndIncrementer(size_t i, std::string_view item);
};