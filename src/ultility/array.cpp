#include "array.hpp"

namespace user {

Array::Array(size_t size, size_t lengthOfItem) : _lengthOfItem(lengthOfItem) {
    size_t sizePerItem = u8size / lengthOfItem;
    size_t newSize = (size + sizePerItem - 1) / sizePerItem;
    _vector.assign(newSize, 0);
}

void Array::set(size_t index) {
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

void Array::printBits(uint8_t value) {
    for (int i = 7; i >= 0; --i) {
        uint8_t bit = (value & (1 << i)) ? 1 : 0;
        printf("%hhu", bit);
    }
}

void Array::print() {
    for (const auto& value : _vector) {
        printBits(value);
    }
}

size_t Array::size() { return _vector.size(); }

}  // namespace user