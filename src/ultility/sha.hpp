#pragma once
#include <iostream>

namespace SHA {
std::string sha256(std::string_view input);
std::array<std::size_t, 5> createHashArray(std::string_view input);
};  // namespace SHA