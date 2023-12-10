#ifndef UTILITY_H
#define UTILITY_H

#include <bits/stdc++.h>
#include "memotable.h"
using namespace std;

extern AVLTree avl;
extern int MAX_NODE;

void printInOrder();
void creationAVL();
// void write_to_sst(vector<SSTNode>&result);
void flush_to_sst();
// void write_index_tree(vector<SSTNode>&result);
// void create_index_tree(vector<SSTNode>&result);
void load_from_sst();

#endif
