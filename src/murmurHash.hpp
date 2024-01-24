#include <cstdint>
#include <functional>
#include <vector>

uint64_t murmurHash64A(const uint8_t *key, uint16_t keyLengh, uint64_t seed);

std::vector<std::function<uint64_t(uint8_t* key, uint16_t keyLength)>> murmurHash64A_Vector(uint8_t n);