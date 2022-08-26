#ifndef LAB3_AVL_TREE_H
#define LAB3_AVL_TREE_H

#include "binary-search-tree.h"

class AVLTree : public BinarySearchTree {
public:
    bool insert(DataType val);
    bool remove(DataType val);

private:

    static bool isBalanced(Node* node); // function to determine whether the tree is balanced
    void balanceSubTree(Node* alpha); // function to balance a subtree
    static Node* search(Node* root, DataType val); // function that returns a pointer to a given Node
    void rotateRight(Node* root);
    void rotateLeft(Node* root);
    void rotateLeftRight(Node* root);
    void rotateRightLeft(Node* root);
};

#endif
