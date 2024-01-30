#pragma once
#include <cstdint>
#include <span>
#include <cstring>
#include "btree.hpp"
#include "utility.hpp"

#define NODE_SIZE 4096
#define POINTER_SIZE sizeof(void *) // Pointer size in byte

struct BTreeNode;
struct BTreeNodeHeader;
struct PageSlot;

struct PageSlot
{
    PageSlot() = default;
    PageSlot(uint16_t offset, uint16_t key_length, uint16_t value_length);

    uint16_t offset;
    uint16_t key_length;
    uint16_t value_length;
};

struct FenceKeySlot {
    FenceKeySlot() = default;
    uint16_t offset;
    uint16_t len;
};

struct BTreeNodeHeader
{

    BTreeNodeHeader(bool is_leaf);
    
    union
    {
        BTreeNode *right_most_child; // right most child of an inner node
        BTreeNode *next_leaf_node;   // right sibling of a leaf node
    };

    bool is_leaf;
    uint16_t count;       // number of tuples
    uint16_t space_used;  // total size of all keys & values
    uint16_t free_offset; // prepend record from this offset
    
    // lower_fence < all keys <= upper_fence
    // read/write fences are similar to that of PageSlot
    uint16_t prefix_len;
    FenceKeySlot lower_fence;
    FenceKeySlot upper_fence;
};

struct alignas(NODE_SIZE) BTreeNode : public BTreeNodeHeader
{
    static constexpr uint32_t CONTENT_SIZE = NODE_SIZE - sizeof(BTreeNodeHeader);

    BTreeNode(bool is_leaf);

    PageSlot slots[CONTENT_SIZE / sizeof(PageSlot)];
    
    std::span<uint8_t> getKey(uint16_t pos);

    std::span<uint8_t> getValue(uint16_t pos);

    std::vector<uint8_t> getPrefix();

    std::span<uint8_t> get_lowerFence();

    std::span<uint8_t> get_upperFence();

    bool replaceChild(uint16_t pos, BTreeNode *child);

    int insert(uint8_t *key, uint16_t keyLength, uint8_t *value, uint16_t valueLength);

    void insert_fenceKey(uint8_t *key, uint16_t keyLength, bool upper);

    bool remove(int pos);

    bool space(uint16_t keyLength, uint16_t valueLength);

    bool compaction(uint16_t keyLength, uint16_t valueLength);

    int insert_index(uint8_t *key, uint16_t keyLength);

    int binaryseach(uint8_t *key, uint16_t keyLength);

    std::pair<bool, int> binaryseach_helper(uint8_t *key, uint16_t keyLength, uint16_t lower, uint16_t upper);

    void clear();

    bool scan(int from, uint8_t *keyOut, const std::function<bool(unsigned int, uint8_t *, unsigned int)> &found_callback);
};
