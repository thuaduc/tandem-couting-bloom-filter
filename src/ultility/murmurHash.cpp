#include "murmurHash.hpp"

uint64_t murmurHash64A(uint8_t *key, uint16_t keyLengh, uint64_t seed) {
    // MurmurHash64A
    return 0;
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
