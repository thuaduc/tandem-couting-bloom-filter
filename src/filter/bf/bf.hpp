#pragma once
#include <array>

template <typename T, unsigned N, unsigned K>
struct BloomFilter {
    std::array<T, N> filter;
    std::array<std::function<uint32_t(uint32_t[])>, K> hashFunctions;
    BloomFilter() : filter {}
};