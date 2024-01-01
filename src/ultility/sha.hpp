#pragma once
#include <iostream>

namespace SHA {
std::string sha256(const std::string& input);
std::array<std::string, 5> createHashArray(const std::string& input);
};  // namespace SHA