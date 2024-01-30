#pragma once
#include <cstdint>
#include <functional>
#include <stack>
#include <cstring>
#include <map>
#include <vector>
#include <span>
#include <stdio.h>
#include "slotted_page.hpp"
#include "utility.hpp"


struct BTreeNode;

struct BTree
{
  BTreeNode *root;
  BTree();
};

typedef std::pair<BTreeNode*, int> parent;

// create a new tree and return a pointer to it
BTree *btree_create();

// destroy a tree created by btree_create
void btree_destroy(BTree *);

void btree_destroy_helper(BTreeNode *node);

// return true iff the key was present
bool btree_remove(BTree *tree, uint8_t *key, uint16_t keyLength);

bool btree_remove_helper(BTreeNode *node, uint8_t *key, uint16_t keyLength);

// replaces exising record if any
void btree_insert(BTree *tree, uint8_t *key, uint16_t keyLength, uint8_t *value,
                  uint16_t valueLength);

void btree_insert_splitted_node(std::vector<uint8_t> promKey, BTreeNode* &left, BTreeNode* &right, uint8_t *key, uint16_t keyLength, uint8_t *value, uint16_t valueLength);

BTreeNode* btree_insert_helper(std::stack<parent>& traverse, BTreeNode *node, uint8_t *key, uint16_t keyLength, uint8_t *value, uint16_t valueLength);

BTreeNode* backtrack(std::stack<parent> &traverse, BTreeNode *node, uint8_t *key, uint16_t keyLength, uint8_t *value, uint16_t valueLength);

std::pair<std::vector<uint8_t>, BTreeNode *> btree_split_node(BTreeNode *first);

int btree_split_index(BTreeNode* node);

void btree_compaction_node(BTreeNode *node, std::vector<uint8_t> prefix);

std::pair<BTreeNode*, int> signpost(BTreeNode *node, uint8_t *key, uint16_t keyLength);

// returns a pointer to the associated value if present, nullptr otherwise
uint8_t *btree_lookup(BTree *tree, uint8_t *key, uint16_t keyLength,
                      uint16_t &payloadLengthOut);

uint8_t *btree_lookup_helper(BTreeNode *node, uint8_t *key, uint16_t keyLength,
                      uint16_t &payloadLengthOut);

// invokes the callback for all records greater than or equal to key, in order.
// the key should be copied to keyOut before the call.
// the callback should be invoked with keyLength, value pointer, and value
// length iteration stops if there are no more keys or the callback returns
// false.
void btree_scan(BTree *tree, uint8_t *key, unsigned keyLength, uint8_t *keyOut,
                const std::function<bool(unsigned int, uint8_t *, unsigned int)>
                    &found_callback);
                    
void btree_scan_helper(BTreeNode* node, uint8_t *key, unsigned keyLength, uint8_t *keyOut, const std::function<bool(unsigned int, uint8_t *, unsigned int)> &found_callback);

