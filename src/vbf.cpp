#include "vbf.hpp"

VariableCoutingBloomFilter::VariableCoutingBloomFilter(size_t m, uint8_t k,
                                                       uint8_t L_set)
    : m{m * 8},
      k{k},
      filter(m * 8),
      f_set{murmurHash64A_Vector(k)},
      g_set{murmurHash64A_Vector(k)},
      L_set{L_set} {
    if (2 > L_set || (L_set & (L_set - 1)) != 0 || 32 < L_set) {
        std::cerr << "L ≥ 2 should be a positive integer of the form L = 2^i"
                  << std::endl;
        std::cerr << "L < should be (for our implementation) <= 32"
                  << std::endl;
        exit(0);
    }
}

void VariableCoutingBloomFilter::insert(uint8_t *key, uint16_t keyLength) {
    for (uint8_t i = 0; i < k; ++i) {
        auto [index, varIncrement] = getVBFvalues(i, key, keyLength);
        increment(index, varIncrement);
    }
}
bool VariableCoutingBloomFilter::lookup(uint8_t *key, uint16_t keyLength) {
    for (uint8_t i = 0; i < k; ++i) {
        auto [index, varIncrement] = getVBFvalues(i, key, keyLength);
        int diff = filter.at(index) - varIncrement;
        if (diff < 0 || (1 <= diff && diff < L_set)) {
            return false;
        }
    }
    return true;
}
bool VariableCoutingBloomFilter::remove(uint8_t *key, uint16_t keyLength) {
    if (!lookup(key, keyLength)) {
        return false;
    }

    for (uint8_t i = 0; i < k; ++i) {
        auto [index, varIncrement] = getVBFvalues(i, key, keyLength);
        decrement(index, varIncrement);
    }
    return true;
}

void VariableCoutingBloomFilter::increment(size_t index, uint8_t varIncrement) {
    if (UINT8_MAX - varIncrement < filter.at(index)) {
        std::cerr << "Counter overflow by couting bloom filter" << std::endl;
        return;
    }
    filter.at(index) += varIncrement;
}
void VariableCoutingBloomFilter::decrement(size_t index, uint8_t varIncrement) {
    if (filter.at(index) < varIncrement) {
        std::cerr << "Counter underflow by couting bloom filter" << std::endl;
        return;
    }
    filter.at(index) -= varIncrement;
}

std::pair<size_t, uint8_t> VariableCoutingBloomFilter::getVBFvalues(
    size_t i, uint8_t *key, uint16_t keyLength) {
    return std::make_pair(
        f_set.at(i)(key, keyLength) % m,
        (g_set.at(i)(key, keyLength) % L_set) + L_set  //[L-set - 2*L_set-1]
    );
}