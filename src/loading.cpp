#include "loading.h"


Collection::Collection(string name)
{
    this->name = name;
}

Db::Db(string name)
{
    this->name = name;
}

nandaDB::nandaDB(string name, vector<Db> Databases)
{
    this->name = name;
    for( auto db : Databases){
        this->Databases[db.name] = db;
    }
}

struct nandaDB NandaDB;