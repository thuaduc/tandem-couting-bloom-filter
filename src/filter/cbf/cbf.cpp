#include "cbf.hpp"

CountingBloomFilter::CountingBloomFilter(size_t size) : BloomFilter(size) {
    _filter.assign(size * 4, 0);
}

bool CountingBloomFilter::add(std::string_view item) {
    for (size_t i = 0; i < nHashFunctions; ++i) {
        size_t pos = hashFunctions[i](item) % this->_size;
        incrementCounterByOne(pos);
    }
    return true;
}

bool CountingBloomFilter::remove(std::string_view item) {
    if (!lookup(item)) {
        return false;
    }

    for (size_t i = 0; i < nHashFunctions; ++i) {
        auto pos = hashFunctions[i](item) % this->_size;
        decrementCounterByOne(pos);
    }
    return true;
}

bool CountingBloomFilter::lookup(std::string_view item) {
    for (size_t i = 0; i < nHashFunctions; ++i) {
        auto pos = hashFunctions[i](item) % this->_size;
        if (!isCounterSet(pos)) {
            return false;
        }
    }
    return true;
}

void CountingBloomFilter::checkIndexValid(size_t pos) {
    if (pos >= _filter.size()) {
        printf("pos: %zu size: %zu\n", pos, _filter.size());
        exit(0);
    }
}

void CountingBloomFilter::incrementCounterByOne(size_t index) {
    size_t pos = index / 2;
    checkIndexValid(pos);

    uint8_t current, mask;

    // increment high part of uint8
    if (index % 2 != 0) {
        current = (_filter.at(pos) & highMask) >> 4;
        if (current == 15) {
            std::cerr << "Counter overflow by couting bloom filter"
                      << std::endl;
            exit(0);
        }
        mask = _filter.at(pos) & lowMask;
        _filter.at(pos) = (++current << 4) | mask;

    }
    // increment low part of uint8
    else {
        current = _filter.at(pos) & lowMask;
        if (current == 15) {
            std::cerr << "Counter overflow by couting bloom filter"
                      << std::endl;
            exit(0);
        }
        mask = _filter.at(pos) & highMask;
        _filter.at(pos) = ++current | mask;
    }
}

bool CountingBloomFilter::decrementCounterByOne(size_t index) {
    size_t pos = index / 2;
    checkIndexValid(pos);
    uint8_t current, mask;

    // decrement high part of uint8
    if (index % 2 != 0) {
        current = (_filter.at(pos) & highMask) >> 4;
        if (current == 0) {
            return false;
        }
        mask = _filter.at(pos) & lowMask;
        _filter.at(pos) = (--current << 4) | mask;

    }
    // decrement low part of uint8
    else {
        current = _filter.at(pos) & lowMask;
        if (current == 0) {
            return false;
        }
        mask = _filter.at(pos) & highMask;
        _filter.at(pos) = --current | mask;
    }

    return true;
}

bool CountingBloomFilter::isCounterSet(size_t index) {
    size_t pos = index / 2;
    checkIndexValid(pos);
    uint8_t current;

    if (index % 2 != 0) {
        current = (_filter.at(pos) & highMask) >> 4;
    } else {
        current = _filter.at(pos) & lowMask;
    }
    return current != 0;
}
