#include "memotable.h"

SSTNode::SSTNode() : id(0) {}

SSTNode::SSTNode(int key, const std::vector<std::pair<std::string, std::string>>& pairs) : id(key) {
    for(auto pair : pairs){
        this->pairs[pair.first] = pair.second;
    }
}


IndexNode::IndexNode(int _id,long long _location, size_t _structSize) :id(_id), location(_location), structSize(_structSize) {}

IndexNode::IndexNode(){}

Node::Node() : id(0), height(0), left(nullptr), right(nullptr), schemaSize(0) {}

Node::Node(int key, vector<pair<string, string>>&pairsVal)
    : id(key),pairs(pairsVal) , height(1), left(nullptr), right(nullptr) {}

int AVLTree::getHeight(Node *node){
    return (node) ? node->height : 0;
}

int AVLTree::getBalanceFactor(Node *node){
    return (node) ? getHeight(node->left) - getHeight(node->right) : 0;
}

Node *AVLTree::rotateRight(Node *node){
    Node *x = node->left;
    Node *y = x->right;

    x->right = node;
    node->left = y;

    node->height = max(getHeight(node->right), getHeight(node->left)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

Node *AVLTree::rotateLeft(Node *node){
    Node *x = node->right;
    Node *y = x->left;
    x->left = node;
    node->right = y;
    node->height = max(getHeight(node->right), getHeight(node->left)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

Node* AVLTree::insert(Node* node, int key, vector<pair<string, string>>&pairs) {
    if (!node) {
        return new Node(key, pairs);
    }

    if (key < node->id) {
        node->left = insert(node->left, key, pairs);
    } else if (key > node->id) {
        node->right = insert(node->right, key, pairs);
    } else {
        return node;
    }

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int balance = getBalanceFactor(node);

    if (balance > 1 && key < node->left->id) {
        if (node->left != nullptr) {
            return rotateRight(node);
        }
    }

    if (balance < -1 && key > node->right->id) {
        if (node->right != nullptr) {
            return rotateLeft(node);
        }
    }

    return node;
}


void AVLTree::insert(int id, vector<pair<string, string>>&pairs){
    root = insert(root, id, pairs);
}

void AVLTree::printInOrder(Node* node) {
    if (node) {
        printInOrder(node->left);
        cout<<"Person with id "<<node->id<<" has ";
        for(auto filed : node->pairs){
            cout<<filed.first<<" with value "<<filed.second<<" ";
        }
        printInOrder(node->right);
    }
}

void AVLTree::printInOrder(){
    printInOrder(this->root);
}