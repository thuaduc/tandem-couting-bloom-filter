#include "printTree.hpp"

void printSpan(const std::span<uint8_t> &mySpan, FILE *out)
{
  fprintf(out, "[");
  for (const auto &element : mySpan)
  {
    fprintf(out, "%c", element);
  }
  fprintf(out, "]");
}

void printInnerNode(BTreeNode *node, bool root, FILE *out)
{
  if (root)
  {
    fprintf(out, "ROOT (%p): ", static_cast<void*>(node));
  }
  else
  {
    fprintf(out, "INNER NODE (%p): ", static_cast<void*>(node));
  }
  for (int i = 0; i < node->count; ++i)
  {
    fprintf(out, "Key: ");
    printSpan(node->getKey(i), out);
    fprintf(out, "  ");
  }
  fprintf(out, "\nChildren:[");
  for (int i = 0; i < node->count; ++i)
  {
    auto child = spanToPointer(node->getValue(i));
    fprintf(out, "%p, ", static_cast<void*>(child));
  }
  fprintf(out, "%p]\n", static_cast<void*>(node->right_most_child));
}

void printLeaf(BTreeNode *leaf, FILE *out)
{
  fprintf(out, "Leaf (%p) : ", static_cast<void*>(leaf));
  for (int i = 0; i < leaf->count; ++i)
  {
    fprintf(out, "Key: ");
    printSpan(leaf->getKey(i), out);
    fprintf(out, "  ");
  }
  fprintf(out, "\nNEXT:[%p]\n", static_cast<void*>(leaf->next_leaf_node));
}

void printTree_helper(std::queue<BTreeNode *> &queue, FILE *out);

void printTree(BTree *tree)
{
  FILE *out = fopen("tree", "w");
  std::queue<BTreeNode *> queue;
  queue.push(tree->root);
  printTree_helper(queue, out);
  fclose(out);
}
void printFence(BTreeNode *node, FILE *out)
{
  fprintf(out, "[LOWER_FENCE:");
  printSpan(node->get_lowerFence(), out);
  fprintf(out, ", UPPER_FENCE:");
  printSpan(node->get_upperFence(), out);
  fprintf(out, ", PREFLENGTH: %d", node->prefix_len);
  fprintf(out, ", PREFIX: ");
  auto tmp = node->getPrefix();
  auto span = std::span{tmp.data(), tmp.size()};
  printSpan(span, out);
  fprintf(out, "]\n");
}


void printTree_helper(std::queue<BTreeNode *> &queue, FILE *out)
{
  BTreeNode *current;
  bool root = true;

  while (!queue.empty())
  {
    current = queue.front();
    if (current->is_leaf)
    {
      printLeaf(current, out);
    }
    else
    {
      printInnerNode(current, root, out);
      for (int i = 0; i < current->count; ++i)
      {
        queue.push(spanToPointer(current->getValue(i)));
      }
      queue.push(current->right_most_child);
    }
    printFence(current, out);
    queue.pop();
    fprintf(out, "\n");
    fprintf(out, "\n");
    root = false;
  }
}
