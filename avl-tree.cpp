#include <algorithm>
#include "avl-tree.h"
#include <stack>


/**
 * Returns the max height of a given node
 *
 */
int height(AVLTree::Node *T){
    if (T == nullptr) return -1;
    else if (T->left == nullptr && T->right == nullptr) return 0;
    else return 1 + std::max(height(T->left), height(T->right));
}

/** Returns whether a given node is balanced
 *
 */
bool AVLTree::isBalanced(AVLTree::Node *T)
{
    if (T == nullptr) return true;
    return abs(height(T->left) - height(T->right)) < 2;
}


/** Returns a pointer to the tree node with a particular value
 *
 * - we can assume the search value will always be in the tree, given that this
 * function is only called after a value is successfully inserted
*/
AVLTree::Node* AVLTree::search(AVLTree::Node* root, int val){

    AVLTree::Node* current = root;

    while (current != nullptr) {
        if (current->val == val) return current;
        else if (val < current->val) current = current->left;
        else current = current->right;
    }

    return current;
}


/**
 * Balances an unbalanced subtree after a node is removed
 */
void AVLTree::balanceSubTree(Node* alpha) {

    // find the root of the subtree of alpha with the greatest height
    Node* A;
    if (height(alpha->left) >= height(alpha->right)) A = alpha->left;
    else A = alpha->right;

    // find the root of the subtree of A with the greatest height
    Node* B;
    if (height(A->left) >= height(A->right)) B = A->left;
    else B = A->right;

    // Case 1: If A < alpha and B < A (or left/right same height)
    if (A == alpha->left && (B == A->left || height(A->left) == height(A->right))) rotateRight(alpha);

    // Case 2: If A > alpha and B > A (or left/right same height)
    else if (A == alpha->right && (B == A->right || height(A->left) == height(A->right))) rotateLeft(alpha);

    // Case 3: If A < alpha and B > A
    else if (A == alpha->left && B == A->right) rotateLeftRight(alpha);

    // Case 4: If A > alpha and B < A
    else if (A == alpha->right && B == A->left) rotateRightLeft(alpha);

}

/**
 * AVL Insert function that maintains the balance of a tree after inserting a node
 */
bool AVLTree::insert(DataType val) {

    // inserts value and returns false is not inserted
    if (!this->BinarySearchTree::insert(val)) return false;

    // get the pointer for the node you just inserted
    Node* root = this->getRootNode();
    Node* just_inserted = search(root, val);

    // Special case: If the node just_inserted is the root node, then the tree will be balanced
    if (just_inserted->val == root->val) return true;

    // General case: We must find the pointer to the unbalanced ancestor that is closest to the node just_inserted.
    Node* temp = root;
    Node* temp_parent;
    Node* alpha = nullptr;

    // loop through the ancestors of the inserted node (just_inserted)
    do {
        temp_parent = temp;

        if (just_inserted->val > temp->val) temp = temp->right;
        else if (just_inserted->val < temp->val) temp =  temp->left;

        // determine whether this particular ancestor is unbalanced
        int leftHeight = height(temp_parent->left);
        int rightHeight = height(temp_parent->right);
        if( abs(leftHeight - rightHeight) >= 2 ) alpha = temp_parent;

    } while ( temp->val != just_inserted->val );

    if(!alpha) return true;

    // Perform AVL rotation based on cases
    if (just_inserted->val < alpha->val && just_inserted->val < alpha->left->val) rotateRight(alpha);
    else if (just_inserted->val > alpha->val && just_inserted->val > alpha->right->val) rotateLeft(alpha);
    else if (just_inserted->val < alpha->val && just_inserted->val > alpha->left->val) rotateLeftRight(alpha);
    else if (just_inserted->val > alpha->val && just_inserted->val < alpha->right->val) rotateRightLeft(alpha);

    return true;
}

/**
 * AVL Remove function that maintains the balance of a tree after removing a node
 */
bool AVLTree::remove(DataType val) {

    // removes value and returns false if not removed
    if (!this->BinarySearchTree::remove(val)) return false;

    // if the tree is empty, no need to re-balance
    Node *current = this->getRootNode();
    if (current == nullptr) return true;

    // otherwise, search for the parent of the node you just removed, adding all of its ancestors into a stack
    std::stack<Node*> s;

    do {
        s.push(current);
        if (val < current->val) current = current->left;
        else if (val > current->val) current = current->right;
        else return false; // something has gone wrong and the value was not deleted
    } while (current != nullptr);

    // get the parent of current
    Node* alpha;

    while (!s.empty()) {
        alpha = s.top(); // get the next ancestor from the stack
        s.pop();

        if (!isBalanced(alpha)) balanceSubTree(alpha); // check if this ancestor is unbalanced, and balance it if not
    }

    return true;

}



void AVLTree::rotateRight(Node* alpha) {

    // perform right rotation
    Node** pT = getRootNodeAddress();
    Node *A = alpha->left;
    alpha->left = A->right;
    A->right = alpha;

    // If alpha was the root of the whole tree, make A to be the new root.
    if (*pT == alpha) {
        *pT = A;
    }
    else {

        // find the parent of alpha
        Node *temp = *pT;
        Node **parentPointerToAlpha = nullptr;

        do{
            if (alpha->val > temp->val)  {
                parentPointerToAlpha = &temp->right;
                temp = temp->right;
            }
            else if (alpha->val < temp->val) {
                parentPointerToAlpha = &temp->left;
                temp = temp->left;
            }
        } while (temp->val != alpha->val);

        // set alpha's parent to be the parent of A
        *parentPointerToAlpha = A;
    }
}

void AVLTree::rotateLeft(Node* alpha) {

    // perform left rotation
    Node** pT = getRootNodeAddress();
    Node *A = alpha->right;
    alpha->right = A->left;
    A->left = alpha;


    // If alpha was the root of the whole tree, make A to be the new root.
    if (*pT == alpha) {
        *pT = A;
    }
    else {

        // find the parent of alpha
        Node *temp = *pT;
        Node **parentPointerToAlpha = nullptr;

        do{
            if (alpha->val > temp->val)  {
                parentPointerToAlpha = &temp->right;
                temp = temp->right;
            }
            else if (alpha->val < temp->val) {
                parentPointerToAlpha = &temp->left;
                temp = temp->left;
            }
        } while (temp->val != alpha->val);

        // set alpha's parent to be the parent of A
        *parentPointerToAlpha = A;
    }
}

void AVLTree::rotateLeftRight(Node* alpha) {
    Node *A = alpha->left;
    rotateLeft(A);
    rotateRight(alpha);
}

void AVLTree::rotateRightLeft(Node* alpha) {
    Node *A = alpha->right;
    rotateRight(A);
    rotateLeft(alpha);
}

