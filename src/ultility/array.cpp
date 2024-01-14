#include "array.hpp"

namespace user {

Array::Array(size_t size, size_t bitsPerItem) : _bitsPerItem(bitsPerItem) {
    if (bitsPerItem != 1 && bitsPerItem != 2 && bitsPerItem != 4 &&
        bitsPerItem != 8) {
        std::cerr << "Bits per item muss be in [1,2,4,8]!" << std::endl;
        exit(0);
    }
    _vector.assign(((size * bitsPerItem) + 7) / 8, 0);
}

void Array::setBitAtIndex(size_t index) {
    size_t pos = index / u8size;
    uint8_t mask = 1 << (index - (pos * u8size));
    _vector.at(pos) = _vector.at(pos) | mask;
}

bool Array::isSet(size_t index) {
    size_t pos = index / u8size;
    uint8_t mask = 1 << (index - (pos * u8size));
    std::cout << "checking isSet: ele: ";
    printBits(_vector.at(pos));
    std::cout << " mask: ";
    printBits(mask);
    std::cout << std::endl;

    return (_vector.at(pos) & mask) != 0;
}

void Array::incrementCounterByOne(size_t index) {
    if (_bitsPerItem != 4) {
        std::cerr << "This method should only be used by couting bloom filter!"
                  << std::endl;
        exit(0);
    }
    size_t pos = index / 2;
    if (pos >= _vector.size()) {
        std::cerr << "Filter out of range" << std::endl;
        exit(0);
    }

    uint8_t current, mask;

    // increment high part of uint8
    if (index % 2 != 0) {
        current = (_vector.at(pos) & highMask) >> 4;
        if (current == 15) {
            std::cerr << "Counter overflow by couting bloom filter"
                      << std::endl;
            exit(0);
        }
        mask = _vector.at(pos) & lowMask;
        _vector.at(pos) = (++current << 4) | mask;

    }
    // increment low part of uint8
    else {
        current = _vector.at(pos) & lowMask;
        if (current == 15) {
            std::cerr << "Counter overflow by couting bloom filter"
                      << std::endl;
            exit(0);
        }
        mask = _vector.at(pos) & highMask;
        _vector.at(pos) = ++current | mask;
    }
}

bool Array::decrementCounterByOne(size_t index) {
    if (_bitsPerItem != 4) {
        std::cerr << "This method should only be used by couting bloom filter!"
                  << std::endl;
        exit(0);
    }
    if ((index / 2) >= _vector.size()) {
        std::cerr << "Filter out of range" << std::endl;
        exit(0);
    }

    size_t pos = index / 2;
    uint8_t current, mask;

    // decrement high part of uint8
    if (index % 2 != 0) {
        current = (_vector.at(pos) & highMask) >> 4;
        if (current == 0) {
            return false;
        }
        mask = _vector.at(pos) & lowMask;
        _vector.at(pos) = (--current << 4) | mask;

    }
    // decrement low part of uint8
    else {
        current = _vector.at(pos) & lowMask;
        if (current == 0) {
            return false;
        }
        mask = _vector.at(pos) & highMask;
        _vector.at(pos) = --current | mask;
    }

    return true;
}

void Array::incrementCounter(size_t index, uint8_t num) {
    if (_vector.at(index) + num >= UINT8_MAX) {
        std::cerr << "Counter overflow by variable couting bloom filter"
                  << std::endl;
    }
    if (index >= _vector.size()) {
        std::cerr << "Filter out of range" << std::endl;
        exit(0);
    }

    _vector.at(index) += num;
}

bool Array::decrementCounter(size_t index, uint8_t num) {
    if (_vector.at(index) == 0 || _vector.at(index) < num) {
        return false;
    }
    if (index >= _vector.size()) {
        std::cerr << "Filter out of range" << std::endl;
        exit(0);
    }

    _vector.at(index) -= num;
    return true;
}

size_t Array::size() { return _vector.size(); }

}  // namespace user