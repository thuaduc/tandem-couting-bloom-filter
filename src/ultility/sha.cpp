#include "sha.hpp"

#include <openssl/sha.h>

#include <iomanip>
#include <iostream>
#include <sstream>

#define NUM_OF_HASH 5
#define HASH_LENGTH 6

namespace SHA {

std::string sha256(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.length());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return ss.str();
}

std::array<std::string, 5> createHashArray(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.length());
    SHA256_Final(hash, &sha256);

    std::array<std::string, 5> output;

    std::stringstream ss;
    for (int i = 0; i < NUM_OF_HASH; ++i) {
        ss.str(std::string());  // Clear the stringstream
        for (int j = 0; j < HASH_LENGTH; ++j) {
            ss << std::hex << std::setw(2) << std::setfill('0')
               << (int)hash[i * HASH_LENGTH + j];
        }
        output.at(i) = ss.str();
    }

    return output;
}

}  // namespace SHA