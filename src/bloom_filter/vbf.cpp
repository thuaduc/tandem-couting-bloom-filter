#include "vbf.hpp"

VariableCoutingBloomFilter::VariableCoutingBloomFilter(uint64_t m, uint8_t k,
                                                       uint8_t L_set): 
      f_set{setOfMurmurHash64A(k)},
      g_set{setOfMurmurHash64A(k)},
      filter(roundUp8(m) >> 3),
      L_set{isPowerOf2(L_set) && (2 <= L_set && L_set <= 32) ? L_set : static_cast<uint8_t>(4)} {}

void VariableCoutingBloomFilter::insert(uint8_t *key, uint16_t keyLength) {
    for (uint8_t i = 0; i < f_set.size(); ++i) {
        auto [index, varIncrement] = getVBFvalues(i, key, keyLength);
        increment(index, varIncrement);
    }
}
bool VariableCoutingBloomFilter::lookup(uint8_t *key, uint16_t keyLength) {
    for (uint8_t i = 0; i < f_set.size(); ++i) {
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

    for (uint8_t i = 0; i < f_set.size(); ++i) {
        auto [index, varIncrement] = getVBFvalues(i, key, keyLength);
        decrement(index, varIncrement);
    }
    return true;
}

void VariableCoutingBloomFilter::increment(size_t index, uint8_t varIncrement) {
    if (filter.at(index) <= UINT8_MAX - varIncrement) {
        filter.at(index) += varIncrement;
    }
}
void VariableCoutingBloomFilter::decrement(size_t index, uint8_t varIncrement) {
    if (varIncrement <= filter.at(index)) {
        filter.at(index) -= varIncrement;
    }
}

std::pair<size_t, uint8_t> VariableCoutingBloomFilter::getVBFvalues(
    size_t i, uint8_t *key, uint16_t keyLength) {
    return std::make_pair(
        f_set.at(i)(key, keyLength) % f_set.size(),
        (g_set.at(i)(key, keyLength) % L_set) + L_set  //[L-set - 2*L_set-1]
    );
}