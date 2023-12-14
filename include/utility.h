#ifndef UTILITY_H
#define UTILITY_H

#include <bits/stdc++.h>
#include "memotable.h"
#include "json.hpp"
using json = nlohmann::json;
using namespace std;

extern const size_t MAX_NODE;

void printInOrder();
void flush_to_sst();
void load_from_sst();

#endif
