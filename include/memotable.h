#ifndef MEMOTABLE_H
#define MEMOTABLE_H

#include <bits/stdc++.h>
using namespace std;

struct SSTNode {
    int id;
    char name[20];
    int age;
    int purchased_value;
    char contact_number[10];

    SSTNode();

    SSTNode(int key, string name, int umar, int pur, string contact);
};

struct IndexNode {
    int id;
    long long location;

    IndexNode();

    IndexNode(int id, long long location);

};

struct Node {
    int id;
    string name;
    int age;
    int purchased_value;
    string contact_number;
    int height;
    Node* right;
    Node* left;

    Node(int key, string naam, int umar, int pur, string contact);
};

class AVLTree {
public:
    Node* root;

    int getHeight(Node* node);
    int getBalanceFactor(Node* node);
    Node* rotateRight(Node* node);
    Node* rotateLeft(Node* node);
    Node* insert(Node* node, int key, string name, int age, int purchased_value, string contact_number);
    void insert(int id, string name, int age, int purchased_value, string contact_number);
    void printInOrder();
    void printInOrder(Node* node);
};

#endif
