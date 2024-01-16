#include "tbf.hpp"

TandemBloomFilter::TandemBloomFilter(size_t size)
    : VariableCoutingBloomFilter(size) {
    _filter.assign(size * 8, 0);
}

std::tuple<size_t, size_t, uint8_t, uint8_t>
TandemBloomFilter::calculatePositionAndHashes(size_t hash) {
    auto pos = hash % _filter.size();
    auto posNeighbour = pos % 2 == 0 ? pos + 1 : pos - 1;
    uint8_t value = 1 + (pos % 8);
    return std::make_tuple(pos, posNeighbour, hash + 8, value);
}

bool TandemBloomFilter::increaseCounterNeighbour(size_t index, uint8_t num) {
    if (_filter.at(index) != 0) {
        return false;
    }

    _filter.at(index) += num;
    return true;
}

bool TandemBloomFilter::add(std::string_view item) {
    bool secondHashSetted = false;
    for (size_t i = 0; i < nHashFunctions; ++i) {
        auto [pos, posNeighbour, hash, hashNeighbour] =
            calculatePositionAndHashes(hashFunctions[i](item));
        increaseCounter(pos, hash);
        if (!secondHashSetted) {
            if (increaseCounterNeighbour(posNeighbour, hashNeighbour)) {
                secondHashSetted = true;
            }
        }
    }
    return true;
}

bool TandemBloomFilter::lookup(std::string_view item) {
    for (size_t i = 0; i < nHashFunctions; ++i) {
        auto [pos, posNeighbour, hash, hashNeighbour] =
            calculatePositionAndHashes(hashFunctions[i](item));

        if (_filter.at(pos) != hash) {
            return false;
        }

        if (_filter.at(posNeighbour) < 8 &&
            _filter.at(posNeighbour) != hashNeighbour) {
            return false;
        }
    }
    return true;
}

bool TandemBloomFilter::remove(std::string_view item) {
    for (size_t i = 0; i < nHashFunctions; ++i) {
        auto [pos, posNeighbour, hash, hashNeighbour] =
            calculatePositionAndHashes(hashFunctions[i](item));

        if (_filter.at(pos) == hash) {
            if (_filter.at(posNeighbour) < 8 &&
                _filter.at(posNeighbour) != hashNeighbour) {
                return false;
            }
            continue;
        }
        if (_filter.at(pos) > hash) {
            _filter.at(pos) -= hash;
        }

        return false;
    }
    return true;
}
