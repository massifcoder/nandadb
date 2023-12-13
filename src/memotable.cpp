#include "memotable.h"
#include "MurmurHash3.h"

IndexNode::IndexNode(int _id, long long _location, size_t _structSize) : id(_id), location(_location), structSize(_structSize) {}

IndexNode::IndexNode() {}

Node::Node() : id(0), height(0), isDeleted(false), left(nullptr), right(nullptr) {}

Node::Node(int key, string &pairsVal, bool isDeleted)
    : id(key), height(1), pairs(pairsVal), isDeleted(isDeleted), left(nullptr), right(nullptr) {}

int AVLTree::getHeight(Node *node)
{
    return (node) ? node->height : 0;
}

int AVLTree::getBalanceFactor(Node *node)
{
    return (node) ? getHeight(node->left) - getHeight(node->right) : 0;
}

Node *AVLTree::rotateRight(Node *node)
{
    Node *x = node->left;
    Node *y = x->right;

    x->right = node;
    node->left = y;

    node->height = max(getHeight(node->right), getHeight(node->left)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

Node *AVLTree::rotateLeft(Node *node)
{
    Node *x = node->right;
    Node *y = x->left;
    x->left = node;
    node->right = y;
    node->height = max(getHeight(node->right), getHeight(node->left)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

Node *AVLTree::update(Node *node, int key, string &pairs)
{
    if (node == nullptr)
    {
        return new Node(key, pairs, false);
    }
    else if (node->id < key)
    {
        node->right = update(node->right, key, pairs);
    }
    else if (node->id > key)
    {
        node->left = update(node->left, key, pairs);
    }
    else
    {
        node->pairs = pairs;
    }
    return node;
}


Node *AVLTree::deleteNode(Node *node, int key, bool &isDeleted)
{
    if (node == nullptr)
    {
        return nullptr;
    }
    else if (node->id < key)
    {
        node->right = deleteNode(node->right, key, isDeleted);
    }
    else if (node->id > key)
    {
        node->left = deleteNode(node->left, key, isDeleted);
    }
    else
    {
        if (node->left == nullptr && node->right == nullptr)
        {
            delete node;
            isDeleted = true;
            return nullptr;
        }
        else if (node->left == nullptr)
        {
            Node *temp = node->right;
            delete node;
            isDeleted = true;
            return temp;
        }
        else if (node->right == nullptr)
        {
            Node *temp = node->left;
            isDeleted = true;
            delete node;
            return temp;
        }
        else
        {
            Node *temp = node->right;
            while (temp->left != nullptr)
            {
                temp = temp->left;
            }
            node->id = temp->id;
            node->pairs = temp->pairs;
            node->right = deleteNode(node->right, temp->id, isDeleted);
        }
    }
    return node;
}

bool AVLTree::deleteNode(int id){
    bool isDeleted = false;
    root = deleteNode(root, id, isDeleted);
    return isDeleted;
}


Node *AVLTree::search(Node *node, int key)
{
    if (!node){
        return nullptr;
    }
    else if (key == node->id){
        return node;
    }
    else if (key < node->id){
        return search(node->left, key);
    }
    else{
        return search(node->right, key);
    }
}

Node *AVLTree::insert(Node *node, int key, string &pairs, bool &isInserted, bool isDeleted)
{
    if (!node){
        isInserted = true;
        return new Node(key, pairs, isDeleted);
    }

    if (key < node->id){
        node->left = insert(node->left, key, pairs, isInserted);
    }
    else if (key > node->id){
        node->right = insert(node->right, key, pairs, isInserted);
    }
    else{
        isInserted = false;
        return node;
    }

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int balance = getBalanceFactor(node);

    if (balance > 1 && key < node->left->id){
        if (node->left != nullptr){
            return rotateRight(node);
        }
    }

    if (balance < -1 && key > node->right->id){
        if (node->right != nullptr){
            return rotateLeft(node);
        }
    }

    return node;
}

bool AVLTree::insert(int id, string &pairs, bool isDeleted){
    bool isInserted = false;
    root = insert(root, id, pairs, isInserted, isDeleted);
    return isInserted;
}

void AVLTree::printInOrder(Node *node){
    if (node){
        printInOrder(node->left);
        cout << "Person with id " << node->id << " has ";
        cout << node->pairs <<" and its deletion status is "<<node->isDeleted<< endl;
        printInOrder(node->right);
    }
}

void AVLTree::printInOrder(){
    printInOrder(this->root);
}


size_t BloomFilter::hash1(int num) {
    uint32_t result;
    MurmurHash3_x86_32(&num, sizeof(int), 0, &result);
    return result % filter.size();
}


size_t BloomFilter::hash2(int num) {
    uint32_t result;
    MurmurHash3_x86_32(&num, sizeof(int), 1, &result);
    return result % filter.size();
}


void BloomFilter::insert(int id){
    filter[hash1(id)] = 1;
    filter[hash2(id)] = 1;
}

bool BloomFilter::contains(int id){
    return filter[hash1(id)] && filter[hash2(id)];
}