#include <algorithm>
#include "avl-tree.h"

// function to get the height of a node
int Height(AVLTree::Node *T){
    if(T == nullptr) return -1;
    else if (T->left == nullptr && T->right == nullptr) return 0;
    else {
        return 1 + std::max(Height(T->left), Height(T->right));
    }
}


// function to get a pointer for given node
// assume it will never be called if the node does not exist (since this is checked in insert function)
AVLTree::Node* AVLTree::search(AVLTree::Node* root, int val){

    // iterative
    AVLTree::Node* current = root;

    while (current != nullptr) {
        if (current->val == val) return current;
        else if (val < current->val) current = current->left;
        else current = current->right;
    }

    return current;
}

bool AVLTree::insert(DataType val) {

    // inserts value and returns false is not inserted
    if (!this->BinarySearchTree::insert(val)) return false;

    // get the pointer for the node you just inserted
    Node* root = this->getRootNode();
    Node* just_inserted = search(root, val);
//    this->print();
    // check if tree is balanced, and balance it if not
    if (!isBalanced(root)) balanceTree(root, just_inserted);
//
//    // update Node Balances
//    this->updateNodeBalance(getRootNode());

    // return whether the tree was successfully balanced
    //return isBalanced(this->getRootNode());
    return true;
}

bool AVLTree::remove(DataType val) {

    // removes value and returns false if not removed
    if (!this->BinarySearchTree::remove(val)) return false;

    // get a pointer for the node you just inserted???

    return false;

    // check if tree is balanced, and balance it if not
    //if (!isBalanced(this->getRootNode())) balanceTree(this->getRootNode());

    // update Node Balances
    this->updateNodeBalance(getRootNode());

    // return whether the tree was successfully balanced
    return isBalanced(this->getRootNode());
}

bool AVLTree::isBalanced(Node* node) {

    // Base case: if the node has no children, it is balanced
    if (node->left == nullptr && node->right == nullptr) return true;

    // if left node is null, see if right node has grandchildren
    if (node-> left == nullptr && node->right != nullptr) {
        return (node->right->left == nullptr && node->right->right == nullptr); // we want no grandchildren
    }
    // if right node is null, see if left node has grandchildren
    else if (node-> left != nullptr && node->right == nullptr) {
        return (node->left->left == nullptr && node->left->right == nullptr); // we want no grandchildren
    }
    else { // check both child nodes
        return isBalanced(node->left) && isBalanced(node->right);
    }

}

void AVLTree::balanceTree(Node* root, Node* just_inserted) {

    // Special case: If the node just_inserted is the root node, then the tree will be balanced
    if (just_inserted->val == root->val) return;

    // General case: We must find the pointer to the unbalanced ancestor that is closest to the node just_inserted.
    Node* temp = root;
    Node* temp_parent = nullptr;
    Node* alpha = nullptr;

    do {
        temp_parent = temp;
        if(just_inserted->val > temp->val) temp = temp->right;
        else if (just_inserted->val < temp->val) temp =  temp->left;
        //now, check if temp_parent is unbalanced.
        int leftHeight = Height(temp_parent->left);
        int rightHeight = Height(temp_parent->right);
        if( abs(leftHeight - rightHeight) >= 2 ) alpha = temp_parent;

    } while ( temp->val != just_inserted->val );

    if(!alpha) return;

    // Perform AVL rotation based on cases
    if (just_inserted->val < alpha->val && just_inserted->val < alpha->left->val) rotateRight(alpha);
    else if (just_inserted->val > alpha->val && just_inserted->val > alpha->right->val) rotateLeft(alpha);
    else if (just_inserted->val < alpha->val && just_inserted->val > alpha->left->val) rotateLeftRight(alpha);
    else if (just_inserted->val > alpha->val && just_inserted->val < alpha->right->val) rotateRightLeft(alpha);

}

AVLTree::Node* AVLTree::rotateRight(Node* alpha) {
//    Node* A = alpha->left;
//    Node* B = A->right;
//    A->right = alpha;
//    alpha->left = B;

    //Input is alpha
    //A is alpha's left
    Node** pT = getRootNodeAddress();
    Node *A = alpha->left;

    //alphas left = A's right
    alpha->left = A->right;

    //A's right = alpha
    A->right = alpha;

    //If alpha was the root of the whole tree, make A to be the new root.
    if (*pT == alpha) {
        *pT = A;
    }
    else {
        //Let alpha's parent take A as the new child.

        //find 	alphas parent
        Node *temp = *pT;
        Node **parentPointerToAlpha = nullptr;
        do{
            if(alpha->val > temp->val)  {
                parentPointerToAlpha = &temp->right;
                temp = temp->right;
            }
            else if (alpha->val < temp->val) {
                parentPointerToAlpha = &temp->left;
                temp = temp->left;
            }
        }
        while (temp->val != alpha->val);

        *parentPointerToAlpha = A;
    }
}

AVLTree::Node* AVLTree::rotateLeft(Node* alpha) {
//    Node* A = alpha->right;
//    Node* B = A->left;
//    A->left = alpha;
//    alpha->right = B;
//    return A;

    Node** pT = getRootNodeAddress();
    //A is alpha's right
    Node *A = alpha->right;

    //alphas right = A's left
    alpha->right = A->left;

    //A's left = alpha
    A->left = alpha;


    //If alpha was the root of the whole tree, make A to be the new root.
    if (*pT == alpha) {
        *pT = A;
    }
    else {
        //Let alpha's parent take A as the new child.

        //find 	alphas parent
        Node *temp = *pT;
        Node **parentPointerToAlpha = nullptr;
        do{
            if(alpha->val > temp->val)  {
                parentPointerToAlpha = &temp->right;
                temp = temp->right;
            }
            else if (alpha->val < temp->val) {
                parentPointerToAlpha = &temp->left;
                temp = temp->left;
            }
        }
        while (temp->val != alpha->val);

        *parentPointerToAlpha = A;
    }
}

AVLTree::Node* AVLTree::rotateLeftRight(Node* alpha) {
//    Node* A = alpha->left;
//    alpha->left = rotateLeft(A);
//    return rotateRight(alpha);

//Input is alpha

    //A is alpha's left
    Node *A = alpha->left;

    //B is A's right


    //Call single left rotation with input (A)
    rotateLeft(A);

    //Call single right rotation with input (alpha)
    rotateRight(alpha);
}

AVLTree::Node* AVLTree::rotateRightLeft(Node* alpha) {
//    Node* A = alpha->right;
//    alpha->right = rotateRight(A);
//    return rotateLeft(alpha);

    //A is alpha's right
    Node *A = alpha->right;

    //Call single right rotation with input (A)
    rotateRight(A);

    //Call single left rotation with input (alpha)
    rotateLeft(alpha);
}

