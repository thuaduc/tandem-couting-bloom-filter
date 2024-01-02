#include "slotted_page.hpp"

//compares to keys lexicographically, returning -1 means key1 < key2, 0 means key1 == key2, 1 means key1 > key2
int compareKeys(uint8_t *key1, uint16_t keyLength1, uint8_t *key2, uint16_t keyLength2)
{
    for (uint16_t i = 0; i < keyLength1 && i < keyLength2; ++i)
    {
        if (key2[i] > key1[i])
        {
            return -1;
        }
        else if (key1[i] > key2[i])
        {
            return 1;
        }
    }
    if (keyLength1 > keyLength2)
    {
        return 1;
    }
    else if (keyLength1 < keyLength2)
    {
        return -1;
    }
    return 0;
}

//returns the BTreeNode pointer to a given span
BTreeNode* spanToPointer(std::span<uint8_t> span)
{
    auto s = span.data();
    uint64_t n = 0;
    for(uint64_t i = 0; i < POINTER_SIZE; ++i){
        (n = (n << 8));
        n += s[i];
    }
    return reinterpret_cast<BTreeNode*>(n);
}

//fills a buffer with a pointer
void pointerToArray(uint8_t* pointer, uint8_t* buffer)
{
    auto n = reinterpret_cast<std::uintptr_t>(pointer);
    for(uint64_t i = 0; i < POINTER_SIZE; ++i){
        buffer[i] = (n >> (56 - 8 * i)) & 0xff;
    }
}

//returns the vector to a given span
std::vector<uint8_t> spanToVec(std::span<uint8_t> span){
    std::vector<uint8_t> res;
    res.reserve(span.size());
    for (const auto &e : span){
        res.emplace_back(e);
    }
    return res;
}

//concatenate  a prefix with a suffix as vector
std::vector<uint8_t> concatenateWord(std::vector<uint8_t> prefix, std::span<uint8_t> suffix){
    prefix.insert(prefix.end(), suffix.begin(), suffix.end());
    return prefix;
}
