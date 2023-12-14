#ifndef LOADING_H
#define LOADING_H

#include <bits/stdc++.h>
#include <chrono>
#include <curses.h>
#include <filesystem>
#include "utility.h"

using namespace std;
namespace fs = std::filesystem;

struct Level{
    string name;
    int numbers;
    Level() = default;
    Level(string name, int numbers);
};

struct Collection
{
    string name;
    vector<Level> levels;
    AVLTree avl;
    Collection() = default;
    Collection(string name, vector<Level> levels);

    bool operator==(const Collection &c);
};

struct Db
{
    string name;
    unordered_map<string, Collection> collections;

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

bool verify();

#endif