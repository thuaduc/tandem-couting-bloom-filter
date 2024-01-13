#include <cstdint>
#include <memory>
#include <functional>

uint64_t murmurHash64A(uint64_t key, uint64_t seed);

std::unique_ptr<std::function<uint64_t(uint64_t)>[]> murmurHash64A_Array(uint32_t n);