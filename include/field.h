#ifndef FIELD_H
#define FIELD_H

#include <bits/stdc++.h>
using namespace std;

struct Field {
    char name[32];
    char type[32];
    size_t size;

    Field(const string& fieldName, const string& fieldType);

    size_t getSizeOfType(const std::string& fieldType);

};

struct TableSchema {
    string tableName;
    vector<Field> fields;
};

struct DatabaseMetadata {
    unordered_map<std::string, TableSchema> tables;
};

void addTable(DatabaseMetadata& metadata, const TableSchema& table);

void printMetadata(const DatabaseMetadata& metadata);

void createSchema(string nameTable);

#endif
