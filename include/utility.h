#ifndef UTILITY_H
#define UTILITY_H

#include <bits/stdc++.h>
#include "memotable.h"
#include "json.hpp"
using json = nlohmann::json;
using namespace std;

extern AVLTree avl;
// extern bloom_parameters parameters;
// extern BloomFilter bloomFilter;
// extern BloomFilter bloomFilter1;
// extern BloomFilter bloomFilter2;
// extern BloomFilter bloomFilter3;
// extern BloomFilter bloomFilter4;
extern int MAX_NODE;

void printInOrder();
void flush_to_sst();
void load_from_sst();

#endif
