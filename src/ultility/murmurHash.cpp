#include "murmurHash.hpp"

uint64_t murmurHash64A(std::string_view k, uint64_t seed) {
    auto key = std::hash<std::string_view>{}(k);
    // MurmurHash64A
    const uint64_t m = 0xc6a4a7935bd1e995;
    const int r = 47;
    uint64_t h = seed ^ (8 * m);
    key *= m;
    key ^= key >> r;
    key *= m;
    h ^= key;
    h *= m;
    h ^= h >> r;
    h *= m;
    h ^= h >> r;
    return h;
}

std::unique_ptr<std::function<uint64_t(std::string_view)>[]> murmurHash64A_Array(uint32_t n){
    static uint32_t count = 0; 
    auto hashFunctions = std::make_unique<std::function<uint64_t(std::string_view)>[]>(n);
    for(uint32_t i = 0; i < n; ++i){
        uint32_t seed = count;
        hashFunctions[i] = [seed](std::string_view key){ return murmurHash64A(key, seed); };
        ++count;
    }
    return hashFunctions;
}
