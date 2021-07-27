#pragma once

#include "BinaryTree.hpp"
#include <vector>
#include <iostream>


template <typename T>
class BinaryTree {

  private:
    class TreeNode {
      public:

        T & data; //will store references to actual data
        TreeNode* left;
        TreeNode* right;

        TreeNode(T & dataArg) : data(dataArg), left(nullptr), right(nullptr) {}
    };
  TreeNode *root_;

  public:

    BinaryTree() : root_(nullptr) {} //Constructor

    BinaryTree(const BinaryTree& other) = delete; //not allowing copy constructor
    ~BinaryTree() {               //Destructor
      destroyWholeTree();
    }

    void createTree(const std::vector<T>& contents); //function to create tree - definition in hppfile

    void destroySubTree(TreeNode* subTreeRootPtr) {
      if (!subTreeRootPtr){
        return;
      }
      destroySubTree(subTreeRootPtr->left);
      destroySubTree(subTreeRootPtr->right);
      subTreeRootPtr->left = nullptr;
      subTreeRootPtr->right = nullptr;

      delete subTreeRootPtr;
    }

    void destroyWholeTree() {
      destroySubTree(root_);
      root_ = nullptr;
    }

    void shout(TreeNode* cur){
      if (cur){
        std::cout<< cur->data << std::endl;
      }
    }
    void preOrder(TreeNode* cur);
    void inOrder(TreeNode* cur);
    void postOrder(TreeNode* cur);

    TreeNode* unsafe_getRootPtr(){
      return root_;
    }

};
