/**
 * An empty BinaryTree.
 *
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

#include <iostream>

#include "BinaryTree.h"
#include "BinaryTree.hpp"

int main() {


  BinaryTree<int>::createTree(btree({3, 5, 6, 9, 12, 18, 21}));

  auto root = btree.unsafe_getRootPtr();
  btree.preOrder(root);

  return 0;
}
