#ifndef LAB3_AVL_TREE_H
#define LAB3_AVL_TREE_H

#include "binary-search-tree.h"

class AVLTree : public BinarySearchTree {
public:
    // Overriden insert and remove functions
    // Do not modify these definitions

    // The AVL tree should be kept balanced after calling just insert or remove.
    bool insert(DataType val);
    bool remove(DataType val);

    // Define additional functions and attributes below if you need
private:

    bool isBalanced(Node* node); // function to determine whether the tree is balanced
    void balanceTree(Node* root, Node* just_inserted); // function to balance the tree
    Node* search(Node* root, DataType val); // function that returns a pointer to a given Node
    Node* rotateRight(Node* root);
    Node* rotateLeft(Node* root);
    Node* rotateLeftRight(Node* root);
    Node* rotateRightLeft(Node* root);
};

#endif  // LAB3_AVL_TREE_H
