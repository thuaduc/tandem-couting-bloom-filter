#pragma once
#include <vector>
#include <span>
#include "slotted_page.hpp"

struct BTreeNode;

//compares to keys lexicographically, returning -1 means key1 < key2, 0 means key1 == key2, 1 means key1 > key2
int compareKeys(uint8_t *key1, uint16_t keyLength1, uint8_t *key2, uint16_t keyLength2);

//returns the BTreeNode pointer to a given span
BTreeNode* spanToPointer(std::span<uint8_t> span);

//fills a buffer with a uint8_t pointer
void pointerToArray(uint8_t* pointer, uint8_t* buffer);

//returns the vector to a given span
std::vector<uint8_t> spanToVec(std::span<uint8_t> span);

//concatenate  a prefix with a suffix as vector
std::vector<uint8_t> concatenateWord(std::vector<uint8_t> prefix, std::span<uint8_t> suffix);