#ifndef UTILITY_H
#define UTILITY_H

#include <bits/stdc++.h>
#include "memotable.h"
using namespace std;

extern AVLTree avl;
extern int MAX_NODE;

void printInOrder();
void flush_to_sst();
void load_from_sst();

#endif
