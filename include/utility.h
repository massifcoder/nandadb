#ifndef UTILITY_H
#define UTILITY_H

#include <bits/stdc++.h>
#include "memotable.h"
using namespace std;

extern AVLTree avl;

void printInOrder();
void creationAVL();
void write_to_sst();
void flush_to_sst(vector<SSTNode>&result);
void create_b_tree();
void create_index_tree();
void preOderTraversal(Node* node, vector<SSTNode>&result);
void load_from_sst();

#endif
