#include "vbf.hpp"

VariableCoutingBloomFilter::VariableCoutingBloomFilter(size_t size)
    : BloomFilter(size) {
    _filter.assign(size * 8, 0);
    secondHashFunction = murmurHash64A_Array(1);
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
VariableCoutingBloomFilter::calculatePositionAndHashValue(
    size_t i, std::string_view item) {
    auto hash = hashFunctions[i](item);
    auto pos = hash % _filter.size();
    uint8_t value = 8 + (hash % 9);

    return std::make_pair(pos, value);
}

bool VariableCoutingBloomFilter::add(std::string_view item) {
    for (size_t i = 0; i < nHashFunctions; ++i) {
        auto [pos, incrementer] = calculatePositionAndHashValue(i, item);
        increaseCounter(pos, incrementer);
    }
    return true;
}

bool VariableCoutingBloomFilter::lookup(std::string_view item) {
    for (size_t i = 0; i < nHashFunctions; ++i) {
        auto [pos, incrementer] = calculatePositionAndHashValue(i, item);
        if (_filter.at(pos) < incrementer ||
            (_filter.at(pos) - incrementer >= 1 &&
             _filter.at(pos) - incrementer < Range)) {
            return false;
        }
    }
    return true;
}

bool VariableCoutingBloomFilter::remove(std::string_view item) {
    for (size_t i = 0; i < nHashFunctions; ++i) {
        auto [pos, incrementer] = calculatePositionAndHashValue(i, item);
        if (_filter.at(pos) >= incrementer) {
            _filter.at(pos) -= incrementer;
        } else {
            return false;
        }
    }
    return true;
}