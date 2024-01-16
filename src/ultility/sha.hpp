#pragma once

#include <array>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string_view>

#include "sha.hpp"

#define NUM_OF_HASH 5
#define HASH_LENGTH 6

namespace SHA {
std::string sha256(std::string_view input);
std::array<std::size_t, 5> createHashArray(std::string_view input);
};  // namespace SHA