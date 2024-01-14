#include <cstdint>
#include <iostream>
#include <vector>

#define u8size 8
#define lowMask 15    // 0b00001111
#define highMask 240  // 0b11110000
namespace user {

class Array {
   public:
    Array(size_t size, size_t bitsPerItem);

    size_t size();

    // set bit at given index to 1
    // only for standard bloom filter
    void setBitAtIndex(size_t index);

    // check if bit is set at given index
    // only for standard bloom filer
    bool isSet(size_t index);

    // increment filter element at given index by one
    // only for counting bloom filter
    void incrementCounterByOne(size_t index);

    // decrement filter element at given index by one
    // only for counting bloom filter
    bool decrementCounterByOne(size_t index);

    // increment filter element at given index by given amount
    // only for variable couting bloom filter
    void incrementCounter(size_t index, uint8_t num);

    // increment filter element at given index by given amount
    // only for variable couting bloom filter
    bool decrementCounter(size_t index, uint8_t num);

    void printBits(uint8_t value);

    void print();

   private:
    std::vector<uint8_t> _vector;
    size_t _bitsPerItem;
};

}  // namespace user