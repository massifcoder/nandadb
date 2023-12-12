#ifndef LOADING_H
#define LOADING_H

#include <bits/stdc++.h>
#include <chrono>
#include <curses.h>
#include <filesystem>
#include "json.hpp"
#include "utility.h"

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;


struct Collection
{
    string name;
    Collection() = default;
    Collection(string name);
};

struct Db
{
    string name;
    vector<Collection> collections;

    Db() = default;
    Db(string name);

};

struct nandaDB{
    string name;
    unordered_map<string, Db> Databases;

    nandaDB() = default;

    nandaDB(string name, vector<Db> Databases);

};

extern struct nandaDB NandaDB;

void POST();

bool verify();

#endif