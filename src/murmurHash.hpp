#include <cstdint>
#include <functional>
#include <vector>
#include <random>

uint64_t murmurHash64A(const uint8_t *key, uint16_t len, uint64_t seed);

std::vector<std::function<uint64_t(uint8_t *key, uint16_t keyLength)>> setOfMurmurHash64A(uint8_t n);