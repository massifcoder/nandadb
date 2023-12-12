#ifndef MEMOTABLE_H
#define MEMOTABLE_H

#include <bits/stdc++.h>
using namespace std;

struct SSTNode {
    int id;
    unordered_map<string, any> pairs;
    SSTNode();
    SSTNode(int key, const vector<pair<string, string>>& pairs);
};

struct IndexNode {
    int id;
    long long location;
    size_t structSize;

    IndexNode();

    IndexNode(int id, long long location, size_t structSize);

};

struct Node {
    int id;
    vector<pair<string, string>> pairs;
    int height;
    Node* right;
    Node* left;
    size_t schemaSize;
    Node();

    Node(int key, vector<pair<string, string>>&pairs);
};

class AVLTree {
public:
    Node* root;

    int getHeight(Node* node);
    int getBalanceFactor(Node* node);
    Node* rotateRight(Node* node);
    Node* rotateLeft(Node* node);
    Node* insert(Node* node, int key, vector<pair<string, string>>&pairs);
    void insert(int id, vector<pair<string, string>>&pairs);
    void printInOrder();
    void printInOrder(Node* node);
};

#endif
