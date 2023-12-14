#include "loading.h"

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

bool Collection::operator==(const Collection &c)
{
    return false;
}

Collection::Collection(string name, vector<Level> levels)
{
    this->name = name;
    this->levels = levels;
}

Level::Level(string name, int numbers)
{
    this->name = name;
    this->numbers = numbers;
}