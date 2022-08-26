#include "binary-search-tree.h"
#include "iostream"
#include <queue>

using namespace std;

BinarySearchTree::Node::Node(DataType newval) {
    val = newval;
    left = nullptr;
    right = nullptr;
    avlBalance = 0;
}

int BinarySearchTree::getNodeDepth(Node* n) const {
    if (n->left == nullptr && n->right == nullptr) return 0; // base case

    int left_subtree_height = 0, right_subtree_height = 0;

    if (n->left != nullptr) left_subtree_height = getNodeDepth(n->left);
    if (n->right != nullptr) right_subtree_height = getNodeDepth(n->right);

    if (left_subtree_height >= right_subtree_height) return 1 + left_subtree_height;
    else return 1 + right_subtree_height;
}

BinarySearchTree::BinarySearchTree() {
    root_ = nullptr;
    size_ = 0;
}

BinarySearchTree::~BinarySearchTree() {
    if (root_ == nullptr) {
        delete root_;
        return;
    }

    Node* current = root_;
    //cout << root_->val << endl;

    while (current->left != nullptr) {
        remove(current->left->val);
    }
    while (current->right != nullptr) {

        remove(current->right->val);
    }

    delete root_;
    root_ = nullptr;
}


unsigned int BinarySearchTree::size() const {
    return size_;
}

BinarySearchTree::DataType BinarySearchTree::max() const {

    Node* current = root_;
    while (current->right != nullptr) current = current->right; // search to the right until max value is found
    return current->val;
}

BinarySearchTree::DataType BinarySearchTree::min() const {

    Node* current = root_;
    while (current->left != nullptr) current = current->left; // search to the left until min value is found
    return current->val;
}

unsigned int BinarySearchTree::depth() const {
    return getNodeDepth(root_);
}

// recursive helper function for printing a tree
void inOrderTraversal(BinarySearchTree::Node *T) {
    if (T == nullptr) return;
    inOrderTraversal(T->left);
    cout << (T->val) << '(' << (T->avlBalance) << ')' << ", ";
    inOrderTraversal(T->right);
}

void BinarySearchTree::print() const {

    // Keep track of the nodes, to print in a
    // breadth first (level order) traversal.
    std::queue<Node*> q;

    // Seed the jobs with the root.
    if (root_ != nullptr)
        q.push(root_);

    // Walk through the tree, adding nodes to the
    // queue level-by-level.
    std::cout << "(";
    while (!q.empty()) {

        // Get the current node from the queue, and remove it.
        Node* cur = q.front();
        q.pop();

        // Print out the nodes value.
        //std::cout << cur->val << '(' << (cur->avlBalance) << ')' <<" ";
        std::cout << cur->val << " ";
        // Check to see if the current node has left or right children,
        // if they exist, add them to the queue.
        if (cur->left != nullptr)
            q.push(cur->left);
        if (cur->right != nullptr)
            q.push(cur->right);
    }
    std::cout << ")" << std::endl;
}

bool BinarySearchTree::exists(DataType val) const {
    Node* current = root_;

    while (current != nullptr) {
        if (val < current->val) current = current->left;
        else if (val > current->val) current = current->right;
        else return true;
    }

    return false;
}

BinarySearchTree::Node* BinarySearchTree::getRootNode() {
    return root_;
}

BinarySearchTree::Node** BinarySearchTree::getRootNodeAddress() {
    return &root_;
}

bool BinarySearchTree::insert(DataType val) {

    // empty BST
    if (root_ == nullptr) {
        root_ = new Node(val);
        size_++;
        return true;
    }

    // general insert
    Node* current = root_;
    Node* parent = nullptr;

    // search for insert location
    while (current != nullptr) {

        parent = current;

        if (val < current->val) current = current->left;
        else if (val > current->val) current = current->right;
        else return false; // the value is already in the tree
    }

    // determine whether to insert at left or right
    if (val < parent->val) parent->left = new Node(val);
    else parent->right = new Node(val);
    size_++;
    return true;

}

bool BinarySearchTree::remove(DataType val) {

    // find the node to delete
    Node* current = root_;
    Node* parent = nullptr;
    bool isLeftChild = false;
    bool isFound = false;

    while (current != nullptr) {

        if (val == current->val) {
            isFound = true;
            break;
        }
        else if (val < current->val) {
            parent = current;
            isLeftChild = true;
            current = current->left;
        }
        else {
            parent = current;
            isLeftChild = false;
            current = current->right;
        }
    }

    if (!isFound) return false;

    // Case 1: leaf node
    if (current->left == nullptr  && current->right == nullptr) {

        // Case 1a: root node with no children
        if (current == root_) {
            delete root_;
            root_ = nullptr;
            size_--;
            return true;
        }

        // Case 1b: general node with no children
        delete current;
        if (isLeftChild) parent->left = nullptr;
        else parent->right = nullptr;
        size_--;
        return true;
    }

    // Case 2: one-child node
    if (current->left != nullptr && current->right == nullptr) {

        // Case 2a: parent node with right child
        if (current == root_) {
            Node* temp = root_;
            root_ = root_->left;
            delete temp;
            size_--;
            return true;
        }

        // Case 2b: general case with right child
        if (isLeftChild) parent->left = current->left;
        else parent->right = current->left;
        delete current;
        size_--;
        return true;
    }

    else if (current->left == nullptr && current->right != nullptr) {

        // Case 2c: parent node with right child
        if (current == root_) {
            Node* temp = root_;
            root_ = root_->right;
            delete temp;
            size_--;
            return true;
        }

        // Case 2d: general case with left child
        if (isLeftChild) parent->left = current->right;
        else parent->right = current->right;
        delete current;
        size_--;
        return true;
    }

    // Case 3: two-child node
    if (current->left != nullptr && current->right != nullptr) {

        // Search for predecessor
        Node* predecessor = current->left;
        isLeftChild = true;
        Node* predecessor_parent = current;

        while (predecessor->right != nullptr) {
            predecessor_parent = predecessor;
            predecessor = predecessor->right;
            isLeftChild = false;
        }

        // Replace the current node's value with the predecessor's value
        current->val = predecessor->val;

        // delete predecessor
        if (predecessor->left == nullptr) { // we already know predecessor->right is nullptr
            if (isLeftChild) predecessor_parent->left = nullptr;
            else predecessor_parent-> right = nullptr;
        }
        else { // right child is empty but left child is not
            if (isLeftChild) predecessor_parent->left = predecessor->left;
            else predecessor_parent->right = predecessor->left;
        }

        delete predecessor;
        size_--;
        return true;

    }

    return false;

}
