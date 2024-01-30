#include "btree.hpp"

inline std::vector<uint8_t> toByteVector(uint8_t *b, unsigned l)
{
  return std::vector<uint8_t>(b, b + l);
}

BTree::BTree() : root{new BTreeNode(true)} {}

// create a new tree and return a pointer to it
BTree *btree_create()
{
  return new BTree();
};

// destroy a tree created by btree_create
void btree_destroy(BTree *t)
{
  btree_destroy_helper(t->root);
  delete t;
}

void btree_destroy_helper(BTreeNode *node)
{
  if(node->is_leaf){
    delete node;
    return;
  }
  std::vector<BTreeNode*> queue;
  queue.reserve(node->count + 1);

  for(int i = 0; i < node->count; ++i){
    auto ptr = spanToPointer(node->getValue(i));
    queue.emplace_back(ptr);
  }
  queue.emplace_back(node->right_most_child);

  for(int i = 0; i < node->count + 1; ++i){
    btree_destroy_helper(queue.at(i));
  }
  delete node;
}

// return true iff the key was present
bool btree_remove(BTree *tree, uint8_t *key, uint16_t keyLength)
{
  return btree_remove_helper(tree->root, key, keyLength);
}


//Recursive helper function for removing a key
bool btree_remove_helper(BTreeNode *node, uint8_t *key, uint16_t keyLength)
{
  if (node->is_leaf)
  {
    int index = node->binaryseach(key + node->prefix_len, keyLength - node->prefix_len);
    if (index < 0)
    {
      return false;
    }
    if(compareKeys(node->getPrefix().data(), node->prefix_len, key, node->prefix_len) != 0){
      return false;
    }
    node->remove(index);
    return true;
  }
  auto [next, index] = signpost(node, key, keyLength);
  return btree_remove_helper(next, key, keyLength);
}

/*
 Inserts a key-value pair into a B-tree, updating the tree structure accordingly.
 If the total length of the key and value exceeds 1024 bytes, the insertion is aborted.
 */

void btree_insert(BTree *tree, uint8_t *key, uint16_t keyLength, uint8_t *value, uint16_t valueLength)
{
  if (valueLength + keyLength > 1024)
  {
    return;
  }
  btree_remove(tree, key, keyLength);  //Remove the key if it already exists in the tree.
  auto s = std::stack<parent>();
  BTreeNode *newRoot = btree_insert_helper(s, tree->root, key, keyLength, value, valueLength); // insert the node
  if (newRoot)
  {
    tree->root = newRoot;
  }
}


/*
Helper function for inserting a key-value pair into a B-tree.
A stack to keep track of the traversal path during insertion.
A pointer to a new root if the splitting propagates to the root; otherwise nullptr.
*/

BTreeNode *btree_insert_helper(std::stack<parent> &traverse, BTreeNode *node, uint8_t *key, uint16_t keyLength, uint8_t *value, uint16_t valueLength)
{
  auto [next, index] = signpost(node, key, keyLength);
  traverse.push({node, index});
  if (node->is_leaf)
  {
    // If the current node is a leaf, initiate the backtrack process.
    return backtrack(traverse, node, key, keyLength, value, valueLength);
  }
  // If the current node is not a leaf, recursively call the function on the next node.
  return btree_insert_helper(traverse, next, key, keyLength, value, valueLength);
}

// backtrack is used to handle overflows that can propagate to higher levels
BTreeNode *backtrack(std::stack<parent> &traverse, BTreeNode *node, uint8_t *key, uint16_t keyLength, uint8_t *value, uint16_t valueLength)
{
  bool split = !node->space(keyLength, valueLength);
  traverse.pop();
  if (!split)
  {
    node->insert(key, keyLength, value, valueLength);
    return nullptr;
  }

  auto [promKey, second] = btree_split_node(node);
  btree_insert_splitted_node(promKey, node, second, key, keyLength, value, valueLength);

  uint8_t buffer[POINTER_SIZE];
  pointerToArray(reinterpret_cast<uint8_t *>(node), buffer);

  if (traverse.empty())
  {
    BTreeNode *newRoot = new BTreeNode(false);
    newRoot->insert(promKey.data(), promKey.size(), buffer, POINTER_SIZE);
    newRoot->right_most_child = second;
    return newRoot;
  }
  auto [p, index] = traverse.top();
  p->is_leaf = false;
  p->replaceChild(index, second);
  return backtrack(traverse, p, promKey.data(), promKey.size(), buffer, POINTER_SIZE);
}

// splits the node into two part, returns a pair with the promoted key as a vector and the other part of the splitted node
std::pair<std::vector<uint8_t>, BTreeNode *> btree_split_node(BTreeNode *first)
{

  BTreeNode *second = new BTreeNode(first->is_leaf);

  if (first->is_leaf)
  {
    second->next_leaf_node = first->next_leaf_node;
    first->next_leaf_node = second;
  }
  else
  {
    second->right_most_child = first->right_most_child;
    first->right_most_child = nullptr;
  }

  int left_end = btree_split_index(first);
  int right_start = left_end + 1;

  auto fprefix = first->getPrefix();

  //fenceKey for second node 
  auto lower_fence = concatenateWord(fprefix, first->getKey(left_end));
  auto upper_fence = first->get_upperFence();

  second->insert_fenceKey(lower_fence.data(), lower_fence.size(), false);
  second->insert_fenceKey(upper_fence.data(), upper_fence.size(), true);

  uint16_t n = first->count;
  for (uint16_t i = right_start; i < n; ++i)
  {
    auto fkey = first->getKey(i);
    auto fvalue = first->getValue(i);

    --first->count;
    first->space_used -= (fkey.size() + fvalue.size());

    auto word = concatenateWord(fprefix, fkey);
    second->insert(word.data(), word.size(), fvalue.data(), fvalue.size());
  }

  if (!first->is_leaf)
  {
    auto rightchild = spanToPointer(first->getValue(left_end));
    first->remove(left_end);
    first->right_most_child = rightchild;
  }

  first->insert_fenceKey(lower_fence.data(), lower_fence.size(), true);
  btree_compaction_node(first, fprefix);
  return {lower_fence, second};
}

// returns the left end (inclusiv) index to split the node into two parts, split index depends on space_used/2
int btree_split_index(BTreeNode *node)
{
  uint32_t sum = 0;
  uint32_t index = 0;
  while (index < node->count)
  {
    auto key = node->getKey(index);
    auto value = node->getValue(index);
    if (sum + key.size() + value.size() > static_cast<uint32_t>(node->space_used / 2 - node->lower_fence.len - node->upper_fence.len))
    {
      break;
    }
    sum += (key.size() + value.size());
    ++index;
  }
  return index;
}

// helper function to insert key and value depending on the promoted key, assuming right and left have enough space
void btree_insert_splitted_node(std::vector<uint8_t> promKey, BTreeNode *&left, BTreeNode *&right, uint8_t *key, uint16_t keyLength, uint8_t *value, uint16_t valueLength)
{
  int c = compareKeys(key, keyLength, promKey.data(), promKey.size());
  if (c <= 0)
  {
    left->insert(key, keyLength, value, valueLength);
  }
  else
  {
    right->insert(key, keyLength, value, valueLength);
  }
}

//Defragments node through compaction; the method can also be used to update all keys if the fence keys have changed
void btree_compaction_node(BTreeNode *node, std::vector<uint8_t> prefix)
{
  std::vector<std::pair<std::vector<uint8_t>, std::vector<uint8_t>>> vecslot;
  vecslot.reserve(node->count + 1);
  vecslot.emplace_back(std::make_pair(spanToVec(node->get_lowerFence()), spanToVec(node->get_upperFence())));

  for (int i = 0; i < node->count; ++i)
  {
    auto key_value = std::make_pair(spanToVec(node->getKey(i)), spanToVec(node->getValue(i)));
    vecslot.emplace_back(key_value);
  }

  node->clear();

  auto [lower_fence, upper_fence] = vecslot.at(0);
  node->insert_fenceKey(lower_fence.data(), lower_fence.size(), false);
  node->insert_fenceKey(upper_fence.data(), upper_fence.size(), true);

  for (uint32_t i = 1; i < vecslot.size(); ++i)
  {
    auto [key, value]= vecslot.at(i);
    auto word = concatenateWord(prefix, key);
    node->insert(word.data(), word.size(), value.data(), value.size());
  }
}

// keys in inner nodes are used as signposts, returns the next node based on the provided key within an inner node of a B-tree
//If the node is a leaf, returns a pair with a null pointer and -1.
std::pair<BTreeNode *, int> signpost(BTreeNode *node, uint8_t *key, uint16_t keyLength)
{
  if (node->is_leaf)
  {
    return {nullptr, -1};
  }

  uint16_t index;
  auto prefix = node->getPrefix();
  int c = compareKeys(key, node->prefix_len, prefix.data(), node->prefix_len);
  if(c < 0){
    index = 0;
  }
  else if(0 < c){
    index = node->count;
  }
  else{
    index = node->insert_index(key + node->prefix_len, keyLength - node->prefix_len);
  }
  
  if (index == node->count)
  {
    return {node->right_most_child, index};
  }
  auto child = node->getValue(index);

  auto ptr = spanToPointer(child);
  return {ptr, index};
}

// returns a pointer to the associated value if present, nullptr otherwise
uint8_t *btree_lookup(BTree *tree, uint8_t *key, uint16_t keyLength,
                      uint16_t &payloadLengthOut)
{
  return btree_lookup_helper(tree->root, key, keyLength, payloadLengthOut);
}

//recursive call until leaf node is reached
uint8_t *btree_lookup_helper(BTreeNode *node, uint8_t *key, uint16_t keyLength,
                             uint16_t &payloadLengthOut)
{
  if (node->is_leaf)
  {
    static uint8_t EmptyKey;
    int i = node->binaryseach(key + node->prefix_len, keyLength - node->prefix_len);
    if (i < 0)
    {
      return nullptr;
    }
    if(compareKeys(node->getPrefix().data(), node->prefix_len, key, node->prefix_len) != 0){
      return nullptr;
    }
    auto v = node->getValue(i);
    payloadLengthOut = v.size();
    if (payloadLengthOut > 0)
    {
      return reinterpret_cast<uint8_t *>(v.data());
    }
    return &EmptyKey;
  }
  auto [next, index] = signpost(node, key, keyLength);
  return btree_lookup_helper(next, key, keyLength, payloadLengthOut);
}

// invokes the callback for all records greater than or equal to key, in order.
// the key should be copied to keyOut before the call.
// the callback should be invoked with keyLength, value pointer, and value
// length iteration stops if there are no more keys or the callback returns
// false.
void btree_scan(BTree *tree, uint8_t *key, unsigned keyLength, uint8_t *keyOut,
                const std::function<bool(unsigned int, uint8_t *, unsigned int)>
                    &found_callback)
{
  btree_scan_helper(tree->root, key, keyLength, keyOut, found_callback);
}

void btree_scan_helper(BTreeNode *node, uint8_t *key, unsigned keyLength, uint8_t *keyOut, const std::function<bool(unsigned int, uint8_t *, unsigned int)> &found_callback)
{
  if (!node->is_leaf)
  {
    auto [next, index] = signpost(node, key, keyLength);
    btree_scan_helper(next, key, keyLength, keyOut, found_callback);
    return;
  }

  BTreeNode *current = node;

  key += node->prefix_len;
  keyLength -= node->prefix_len;
  int from = node->insert_index(key, keyLength);

  while (current && current->scan(from, keyOut, found_callback))
  {
    from = 0;
    current = current->next_leaf_node;
  }
}
