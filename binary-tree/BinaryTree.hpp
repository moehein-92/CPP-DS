
#pragma once

#include "BinaryTree.h"


template <typename T>
void BinaryTree<T>::createTree(const std::vector<T>& contents) {
  destroyWholeTree();
  if (contents.empty())
      return;
  root_ = new TreeNode(contents[0]);

  std::queue<TreeNode**> childPPQ; // Why using TreeNode**?
  childPPQ.push(&(root->left));
  childPPQ.push(&(root->right));

  for (int i = 1; i < contents.size(); i++){
    TreeNode** childPP= childPPQ.front(); //
    childPPQ.pop();
    TreeNode*& realchildP = *childPP;   //Don't get this part and why create realChildP?

    realchildP = new TreeNode(contents[i]);  //Why not just do this from beginning?
    childPPQ.push(&(realchildP->left));
    childPPQ.push(&(realchildP->right));
  }
}

template <typename T>
void BinaryTree<T>::preOrder(TreeNode* cur) {
  if (cur){
    shout(cur->data);
    preOrder(cur->left);
    preOrder(cur->right);
  }
}

template <typename T>
void BinaryTree<T>::inOrder(TreeNode* cur) {
  if (cur) {
    inOrder(cur->left);
    shout(cur->data);
    inOrder(cur->right);
  }
}

template <typename T>
void BinaryTree<T>::postOrder(Tree* cur) {
  if (cur) {
    postOrder(cur->left);
    postOrder(cur->right);
    shout(cur->data);
  }
}
