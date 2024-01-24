#include "murmurHash.hpp"

//MurmurHash64A
uint64_t murmurHash64A(const uint8_t *key, uint16_t keyLengh, uint64_t seed) {

    const uint64_t m = 0xc6a4a7935bd1e995;
    const int r = 47;
    uint64_t h = seed ^ (keyLengh * m);
    const uint8_t *end = key + (keyLengh-(keyLengh & 7));

    while(key != end) {
        uint64_t k;
        k = *((uint64_t*)key);
        k *= m;
        k ^= k >> r;
        k *= m;
        h ^= k;
        h *= m;
        key += 8;
    }

    switch(keyLengh & 7) {
        case 7: h ^= (uint64_t)key[6] << 48; break;
        case 6: h ^= (uint64_t)key[5] << 40; break;
        case 5: h ^= (uint64_t)key[4] << 32; break;
        case 4: h ^= (uint64_t)key[3] << 24; break;
        case 3: h ^= (uint64_t)key[2] << 16; break;
        case 2: h ^= (uint64_t)key[1] << 8; break;
        case 1: h ^= (uint64_t)key[0]; break;
    };

    h *= m;
    h ^= h >> r;
    h *= m;
    h ^= h >> r;
    return h;
}

std::vector<std::function<uint64_t(uint8_t* key, uint16_t keyLength)>> murmurHash64A_Vector(uint8_t n){
    static uint32_t count = 0; 
    auto hashFunctions = std::vector<std::function<uint64_t(uint8_t* key, uint16_t keyLength)>>();
    hashFunctions.reserve(n);

    for(uint32_t i = 0; i < n; ++i){
        uint32_t seed = count;
        hashFunctions[i] = [seed](uint8_t *key, uint16_t keyLength){ return murmurHash64A(key, keyLength, seed); };
        ++count;
    }
    
    return hashFunctions;
}
