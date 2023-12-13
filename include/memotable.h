#ifndef MEMOTABLE_H
#define MEMOTABLE_H

#include <bits/stdc++.h>
using namespace std;


struct IndexNode {
    int id;
    long long location;
    size_t structSize;

    IndexNode();

    IndexNode(int id, long long location, size_t structSize);

};

struct Node {
    int id;
    bool isDeleted;
    string pairs;
    int height;
    Node* right;
    Node* left;
    size_t schemaSize;
    Node();
    Node(int key, string &pairs, bool isDeleted);
};

class AVLTree {
public:
    Node* root;

    int getHeight(Node* node);
    int getBalanceFactor(Node* node);
    Node* search(Node* node, int key);
    Node* update(Node* node, int key, string &pairs);
    Node* deleteNode(Node* node, int key, bool &isDeleted);
    Node* rotateRight(Node* node);
    Node* rotateLeft(Node* node);
    Node* insert(Node* node, int key, string &pairs, bool &isInserted, bool isDeleted=false);
    bool insert(int id, string &pairs, bool isDeleted=false);
    bool deleteNode(int id);
    void printInOrder();
    void printInOrder(Node* node);
};


#endif
