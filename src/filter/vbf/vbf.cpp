#include "vbf.hpp"

VariableCoutingBloomFilter::VariableCoutingBloomFilter(size_t size)
    : BloomFilter(size) {
    _filter.assign(size * 8, 0);
}

void VariableCoutingBloomFilter::increaseCounter(size_t index, uint8_t num) {
    if (_filter.at(index) + num >= UINT8_MAX) {
        std::cerr << "Counter overflow by variable couting bloom filter"
                  << std::endl;
    }
    if (index >= _filter.size()) {
        std::cerr << "Filter out of range" << std::endl;
        exit(0);
    }

    _filter.at(index) += num;
}

bool VariableCoutingBloomFilter::decreaseCounter(size_t index, uint8_t num) {
    if (_filter.at(index) == 0 || _filter.at(index) < num ||
        _filter.at(index) - num < 8) {
        return false;
    }
    if (index >= _filter.size()) {
        std::cerr << "Filter out of range" << std::endl;
        exit(0);
    }

    _filter.at(index) -= num;
    return true;
}

std::pair<size_t, uint8_t>
VariableCoutingBloomFilter::calculatePositionAndHashValue(size_t hash) {
    auto pos = hash % _filter.size();
    auto value = 9 + (pos % 8);
    return std::make_pair(pos, value);
}

bool VariableCoutingBloomFilter::add(std::string_view item) {
    for (size_t i = 0; i < nHashFunctions; ++i) {
        auto [pos, hash] =
            calculatePositionAndHashValue(hashFunctions[i](item));
        increaseCounter(pos, hash);
    }
    return true;
}

bool VariableCoutingBloomFilter::lookup(std::string_view item) {
    for (size_t i = 0; i < nHashFunctions; ++i) {
        auto [pos, hash] =
            calculatePositionAndHashValue(hashFunctions[i](item));
        if (_filter.at(pos) == hash) {
            continue;
        }
        if (_filter.at(pos) > 16 && _filter.at(pos) <= 32 &&
            _filter.at(pos) - hash < 8) {
            return false;
        }
    }
    return true;
}

bool VariableCoutingBloomFilter::remove(std::string_view item) {
    for (size_t i = 0; i < nHashFunctions; ++i) {
        auto [pos, hash] =
            calculatePositionAndHashValue(hashFunctions[i](item));
        if (_filter.at(pos) == hash) {
            _filter.at(pos) = 0;
            continue;
        }
        if (_filter.at(pos) > 16 && _filter.at(pos) <= 32) {
            if (_filter.at(pos) - hash < 8) {
                return false;
            } else {
                _filter.at(pos) -= hash;
            }
        }
    }
    return true;
}