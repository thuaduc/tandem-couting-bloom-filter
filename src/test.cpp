#include <cstdint>
#include <iostream>

void printBits(uint8_t value) {
    for (int i = 7; i >= 0; --i) {
        uint8_t bit = (value & (1 << i)) ? 1 : 0;
        printf("%hhu", bit);
    }
    std::cout << std::endl;
}

int main() {
    printBits(10);
    return 0;
}