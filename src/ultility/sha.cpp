#include "sha.hpp"

#include <openssl/evp.h>

#include <array>
#include <iomanip>
#include <iostream>
#include <sstream>

#define NUM_OF_HASH 5
#define HASH_LENGTH 6

namespace SHA {

std::string sha256(const std::string& input) {
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    const EVP_MD* md = EVP_sha256();

    EVP_DigestInit_ex(mdctx, md, nullptr);
    EVP_DigestUpdate(mdctx, input.c_str(), input.length());

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    EVP_DigestFinal_ex(mdctx, hash, &hash_len);

    EVP_MD_CTX_free(mdctx);

    std::stringstream ss;
    for (unsigned int i = 0; i < hash_len; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(hash[i]);
    }

    return ss.str();
}

std::array<std::size_t, NUM_OF_HASH> createHashArray(const std::string& input) {
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    const EVP_MD* md = EVP_sha256();

    EVP_DigestInit_ex(mdctx, md, nullptr);
    EVP_DigestUpdate(mdctx, input.c_str(), input.length());

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    EVP_DigestFinal_ex(mdctx, hash, &hash_len);

    EVP_MD_CTX_free(mdctx);

    std::array<std::size_t, NUM_OF_HASH> output;

    for (int i = 0; i < NUM_OF_HASH; ++i) {
        std::size_t hashValue = 0;
        for (int j = 0; j < HASH_LENGTH; ++j) {
            hashValue = (hashValue << 8) | hash[i * HASH_LENGTH + j];
        }
        output.at(i) = hashValue;
    }

    return output;
}

}  // namespace SHA
