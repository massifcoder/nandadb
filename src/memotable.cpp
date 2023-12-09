#include "memotable.h"


SSTNode::SSTNode(int key, std::string name, int umar, int pur, std::string contact) : id(key), age(umar), purchased_value(pur) {
    strcpy(this->name, name.c_str());
    strcpy(this->contact_number, contact.c_str());
}


Node::Node(int key, string naam, int umar, int pur, string contact)
    : id(key), name(naam), age(umar), purchased_value(pur), contact_number(contact), height(1), left(nullptr), right(nullptr) {}

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

Node* AVLTree::insert(Node* node, int key, string name, int age, int purchased_value, string contact_number) {
    if (!node) {
        return new Node(key, name, age, purchased_value, contact_number);
    }

    if (key < node->id) {
        node->left = insert(node->left, key, name, age, purchased_value, contact_number);
    } else if (key > node->id) {
        node->right = insert(node->right, key, name, age, purchased_value, contact_number);
    } else {
        return node; // Assuming you don't want duplicates
    }

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int balance = getBalanceFactor(node);

    if (balance > 1 && key < node->left->id) {
        return rotateRight(node);
    }

    if (balance < -1 && key > node->right->id) {
        return rotateLeft(node);
    }

    return node;
}


void AVLTree::insert(int id, string name, int age, int purchased_value, string contact_number){
    root = insert(root, id, name, age, purchased_value, contact_number);
}

void AVLTree::printInOrder(Node* node) {
    if (node) {
        printInOrder(node->left);
        cout << node->id << endl;
        printInOrder(node->right);
    }
}

void AVLTree::printInOrder(){
    printInOrder(this->root);
}