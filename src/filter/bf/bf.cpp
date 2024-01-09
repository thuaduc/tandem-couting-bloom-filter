#include "bf.hpp"


BloomFilter::BloomFilter(size_t size) : _size{size}{
    _filter.resize(HELP::calculateFilterLength(size));
}


size_t BloomFilter::size() {
    return _size;
}

std::string BloomFilter::realSizeInMB() {
    float sizeInMB = (static_cast<float>(sizeof(_filter)) + 8) / 1e6;
    return std::to_string(sizeInMB) + " MB";
}

bool BloomFilter::add(const std::string& item) {
    auto hashArray = SHA::createHashArray(item);
    for (size_t i = 0; i < 5; i++) {
        _filter[hashArray.at(i) % _size] = 1;
    }
    return true;
}

bool BloomFilter::search(const std::string& item) {
    auto hashArray = SHA::createHashArray(item);
    for (size_t i = 0; i < 5; i++) {
        if (_filter[hashArray.at(i) % _size] == 0) return false;
    }
    return true;
}
