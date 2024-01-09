#pragma once
#include <bitset>
#include <cmath>

#include "../../ultility/help.hpp"
#include "../../ultility/sha.hpp"
#include <vector>
#include <array>

class BloomFilter {
   public:
    BloomFilter(size_t size);
    size_t size();
    std::string realSizeInMB();
    bool add(const std::string& item);
    bool search(const std::string& item);

   private:
    size_t _size;
    std::vector<bool> _filter;
};
