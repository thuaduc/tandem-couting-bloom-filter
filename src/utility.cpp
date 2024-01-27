#include "utility.hpp"

uint64_t randomSeed64()
{
    static std::mt19937_64 rng(42);
    static std::uniform_int_distribution<uint64_t> distribution(0, UINT64_MAX);
    return distribution(rng);
}

uint64_t roundUp16(uint64_t x)
{
    return ((x + 15) >> 4) << 4;
}

bool isPowerOf2(uint64_t x){
    return (x & (x - 1)) == 0 ;
}