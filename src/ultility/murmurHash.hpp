#include <cstdint>
#include <memory>
#include <functional>
#include <string_view>

uint64_t murmurHash64A(std::string_view k, uint64_t seed);

std::unique_ptr<std::function<uint64_t(std::string_view)>[]> murmurHash64A_Array(uint32_t n);