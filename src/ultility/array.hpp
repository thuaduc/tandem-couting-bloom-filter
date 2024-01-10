#include <cstdint>
#include <iostream>
#include <vector>

#define u8size 8

namespace user {

class Array {
   public:
    Array(size_t size, size_t lengthOfItem);

    size_t size();

    void set(size_t index);

    bool isSet(size_t index);

    // void increase(uint8_t num);

    void printBits(uint8_t value);

    void print();

   private:
    std::vector<uint8_t> _vector;
    size_t _lengthOfItem;
};

}  // namespace user