#include <cstring>
#include <map>
#include <vector>
#include <iostream>
#include <span>
#include <stdio.h>
#include <queue>
#include "slotted_page.hpp"
#include "btree.hpp"

void printSpan(const std::span<uint8_t> &mySpan, FILE *out);

void printInnerNode(BTreeNode *node, bool root, FILE *out);

void printLeaf(BTreeNode *leaf, FILE *out);

void printTree_helper(std::queue<BTreeNode *> &queue, FILE *out);

void printTree(BTree *tree);

void printFence(BTreeNode *node, FILE *out);