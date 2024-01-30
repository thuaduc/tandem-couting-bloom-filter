#include "slotted_page.hpp"

PageSlot::PageSlot(uint16_t offset, uint16_t key_length, uint16_t value_length) : offset{offset}, key_length{key_length}, value_length{value_length} {};

BTreeNodeHeader::BTreeNodeHeader(bool is_leaf) : right_most_child{nullptr}, is_leaf{is_leaf}, count{0}, space_used{0}, free_offset{NODE_SIZE}, prefix_len{0}, lower_fence{}, upper_fence{} {}

BTreeNode::BTreeNode(bool is_leaf) : BTreeNodeHeader(is_leaf){};

//returns the key as a span to a given positon
std::span<uint8_t> BTreeNode::getKey(uint16_t pos)
{
    auto ptr = reinterpret_cast<uint8_t *>(this);
    return {ptr + slots[pos].offset, slots[pos].key_length};
}

//returns the value as a span to a given positon
std::span<uint8_t> BTreeNode::getValue(uint16_t pos)
{
    auto ptr = reinterpret_cast<uint8_t *>(this);
    return {ptr + slots[pos].offset + slots[pos].key_length,
            slots[pos].value_length};
}

//Return prefix from fence keys
std::vector<uint8_t> BTreeNode::getPrefix(){
    auto lower_fence = get_lowerFence();
    auto upper_fence = get_upperFence();
    std::vector<uint8_t> prefix;
    int min = lower_fence.size() < upper_fence.size() ? lower_fence.size() : upper_fence.size();
    prefix.reserve(min);

    for(int i = 0; i < min; ++i){
        if(lower_fence.data()[i] != upper_fence.data()[i]){
            break;
        }
        prefix.emplace_back(lower_fence.data()[i]);
    }
    return prefix;
}

//Return prefix from lower fence key
std::span<uint8_t> BTreeNode::get_lowerFence()
{
    auto ptr = reinterpret_cast<uint8_t *>(this);
    return {ptr + lower_fence.offset,
            lower_fence.len};
}

//Return prefix from upper fence key
std::span<uint8_t> BTreeNode::get_upperFence()
{
    auto ptr = reinterpret_cast<uint8_t *>(this);
    return {ptr + upper_fence.offset,
            upper_fence.len};
}

//if node is not child then it, replaces the child to a given position
bool BTreeNode::replaceChild(uint16_t pos, BTreeNode *child)
{
    if (pos > count || is_leaf)
    {
        return false;
    }
    if (pos == count)
    {
        right_most_child = child;
    }
    else
    {
        auto toReplace = getValue(pos);
        uint8_t buffer[POINTER_SIZE];
        pointerToArray(reinterpret_cast<uint8_t *>(child), buffer);
        std::memcpy(toReplace.data(), buffer, toReplace.size());
    }
    return true;
}

//insert key and value to a node if there is enough space, may invoke compaction to insert key and value
int BTreeNode::insert(uint8_t *key, uint16_t keyLength, uint8_t *value, uint16_t valueLength)
{
    if (!space(keyLength - prefix_len, valueLength))
    {
        return -1;
    }

    if (compaction(keyLength - prefix_len, valueLength))
    {
        btree_compaction_node(this, getPrefix());
    }
    if(prefix_len > keyLength){
        return -1;
    }
    key += prefix_len;
    keyLength -= prefix_len;

    uint16_t offset = (free_offset - keyLength - valueLength);
    PageSlot slot{offset, keyLength, valueLength};

    auto ptr = reinterpret_cast<uint8_t *>(this);
    std::memcpy(ptr + (free_offset - valueLength), value, valueLength);
    free_offset -= valueLength;
    std::memcpy(ptr + (free_offset - keyLength), key, keyLength);
    free_offset -= keyLength;
    space_used += (keyLength + valueLength);

    int index = insert_index(key, keyLength);
    for (int i = count; i > index; --i)
    {
        slots[i] = slots[i - 1];
    }
    slots[index] = slot;
    ++count;
    return index;
}

//insert a new fence key to this node, if parameter upper indicates wether its the lower fence key or upper fence key
void BTreeNode::insert_fenceKey(uint8_t *key, uint16_t keyLength, bool upper){
    
    space_used -= upper_fence.len;

    if (!space(keyLength, 0))
    {
        return;
    }

    if (compaction(keyLength, 0))
    {
        btree_compaction_node(this, getPrefix());
    }

    uint16_t offset = (free_offset - keyLength);
    if(upper){
        upper_fence.offset = offset;
        upper_fence.len = keyLength;
    }
    else{
        lower_fence.offset = offset;
        lower_fence.len = keyLength;        
    }

    auto ptr = reinterpret_cast<uint8_t*>(this);
    std::memcpy(ptr + offset, key, keyLength);
    free_offset -= keyLength;
    space_used += keyLength;
    prefix_len = getPrefix().size();
}


//checks whether there is sufficient space in the node
bool BTreeNode::space(uint16_t keyLength, uint16_t valueLength)
{
    int key_val_length = keyLength + valueLength;
    int total_space = static_cast<int>(sizeof(PageSlot)) + static_cast<int>(key_val_length);
    int space_left = static_cast<int>(CONTENT_SIZE) - static_cast<int>((sizeof(PageSlot) * count)) - static_cast<int>(space_used);
    return total_space <= space_left;
}

/*
Determines the index at which a key should be inserted in the node
*/
int BTreeNode::insert_index(uint8_t *key, uint16_t keyLength)
{
    auto [found, index] = binaryseach_helper(key, keyLength, 0, count);
    return index;
}


//removes the element at the specified position within the node, if the position is valid
bool BTreeNode::remove(int pos)
{
    if (count <= pos || pos < 0)
    {
        return false;
    }

    if (is_leaf && pos == count)
    {
        next_leaf_node = nullptr;
    }
    else if (!is_leaf && pos == count)
    {
        right_most_child = nullptr;
    }
    else
    {
        PageSlot toRemove = slots[pos];
        uint16_t total_length = toRemove.key_length + toRemove.value_length;

        for (int i = pos; i < count - 1; ++i)
        {
            slots[i] = slots[i + 1];
        }
        space_used -= total_length;
        --count;
    }
    return true;
}

/*
Performs a binary search within node to find the index of a given key.
If the key is found, returns its index; otherwise, returns -1.
*/
int BTreeNode::binaryseach(uint8_t *key, uint16_t keyLength)
{
    auto [found, index] = binaryseach_helper(key, keyLength, 0, count);
    if (found)
    {
        return index;
    }
    return -1;
}

std::pair<bool, int> BTreeNode::binaryseach_helper(uint8_t *key, uint16_t keyLength, uint16_t lower, uint16_t upper)
{
    while (lower < upper)
    {
        uint16_t middle = lower + ((upper - lower) / 2);
        auto v = getKey(middle);
        int c = compareKeys(key, keyLength, v.data(), v.size());
        if (c == 0)
        {
            return std::make_pair(true, middle);
        }
        else if (0 < c)
        {
            lower = middle + 1;
        }
        else
        {
            upper = middle;
        }
    }
    return std::make_pair(false, lower);
}

//return true iff compaction is necessary for insertion
bool BTreeNode::compaction(uint16_t keyLength, uint16_t valueLength)
{
    return (static_cast<int>(sizeof(PageSlot)) * static_cast<int>((count + 1)) + static_cast<int>(sizeof(BTreeNodeHeader)) >
            static_cast<int>(free_offset) - (static_cast<int>((keyLength)) + static_cast<int>((valueLength))));
}

// invokes the callback for all records greater than or equal to key, in order.
// the key should be copied to keyOut before the call.
// the callback should be invoked with keyLength, value pointer, and value
// length iteration stops if there are no more keys or the callback returns
// false.
bool BTreeNode::scan(int from, uint8_t *keyOut, const std::function<bool(unsigned int, uint8_t *, unsigned int)> &found_callback)
{
    auto prefix = getPrefix();
    for (int i = from; i < count; ++i)
    {
        auto key = getKey(i);
        auto value = getValue(i);
        auto k = concatenateWord(prefix, key);
        std::memcpy(keyOut, k.data(), k.size());
        if (!found_callback(k.size(), value.data(), value.size()))
        {
            return false;
        }
    }
    return true;
}

//Resets the state of a B-tree node, effectively clearing all existing data.
void BTreeNode::clear(){
    free_offset = NODE_SIZE;
    count = 0;
    space_used = 0;
    upper_fence.len = 0;
    upper_fence.offset = 0;
    lower_fence.len = 0;
    lower_fence.offset = 0;
}
