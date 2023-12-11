// To run file, run : g++ main.cpp src/memotable.cpp src/utility.cpp -I include -lncurses 
// 
#include "utility.h"
#include "field.h"
#include<bits/stdc++.h>
#include <chrono>
using namespace std;
#define username "vishal"
#define pwd "vishal"

struct Collection{
    string name;

    Collection(string name){
        this->name = name;
    }

};

struct Db{
    string name;
    vector<Collection> collections;

    Db() = default;

    Db(string name){
        this->name = name;
    }

};

unordered_map<string, Db> databases;

bool verify(){
    string name, password;
    cout<<"Enter your username."<<endl;
    cin>>name;
    cout<<"Enter your password"<<endl;
    cin>>password;
    if(username == name && password == pwd){
        cout<<"Welcome to Nanda DB.\n"<<endl;
        return 1;
    }
    else{
        cout<<"Wrong credentials"<<endl;
        return 0;
    }
}

void clearScreen() {
    std::cout << "\033[2J\033[H";
}

void help(){
    cout<<"\n\033[1;31mTo find docs, type '--help', to insert value in database type 'add', to search value type 'search' and to delete type 'delete'. To exit the program type 'exit' and to clear screen 'clear'.\033[0m \n"<<endl;
}

void demo(){
    string usermode = "Nanda DB";
    bool login = verify();
    if(!login){
        cout<<"You are failed to use our database."<<endl;
        return;
    }
    string query;
    help();
    cout<<endl;
    while(true){
        cout << "\033[0;32m"<<usermode<<" :~ \033[1;34m";
        getline(cin, query);
        cout<<"\033[0m";
        if(query == "--help"){
            help();
        }
        else if(query == "clear"){
            clearScreen();
        }
        else if(query == "show databases"){
            usermode = "Nanda DB";
            cout<<"\033[1;31mList of databases:\033[0m"<<endl;
            for(auto database : databases){
                cout<<" > "<<database.first<<endl;
            }
            cout<<endl;
        }
        else if(query.substr(0,4) == "use "){
            string database = query.substr(4, query.size()-4);
            if(database == "system"){
                cout<<"\033[1;31mYou have no permission! Its for internal working.\033[0m"<<endl;
            }
            else if(databases.find(database) == databases.end()){
                cout<<"\033[1;31mNo such database present.\033[0m"<<endl;
            }
            else{
                usermode = database;
                cout<<endl;
            }
        }
        else if(query == "add"){
            auto start = std::chrono::high_resolution_clock::now();
            // Start code

            // Code ends here and now vishal will analyze.
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << "Duration: " << duration.count() << " microseconds" << std::endl;
        }
        else if(query == "show collections"){
            if(usermode == "Nanda DB"){
                cout<<"\033[1;31mSelect a database where you want to work.\033[0m \n"<<endl;
            }
            else{
                if(databases.find(usermode) != databases.end()){
                    cout<<"\033[1;31mList of collections in "<<usermode<<" are :\033[0m"<<endl;
                    vector<Collection> collections = databases[usermode].collections;
                    for(auto &collection : collections){
                        cout<<" > "<<collection.name<<endl;
                    }
                    cout<<endl;
                }
            }
        }
        else if(query.substr(0,18) == "create collection "){
            string collection_name = query.substr(18, query.size()-18);
            cout<<collection_name<<endl;
            if(collection_name.size() < 4){
                cout<<"\033[1;31mName should be of size more than or equal to 4.\033[0m \n"<<endl;
            }
            else if(usermode == "Nanda DB"){
                cout<<"\033[1;31mSelect a database where you want to work.\033[0m \n"<<endl;
            }
            else if(databases.find(usermode) == databases.end()){
                cout<<"\033[1;31mNo such database present.\033[0m"<<endl;
            }
            else{
                vector<Collection> &collections = databases[usermode].collections;
                bool isFound = false;
                for(auto &collection : collections){
                    if(collection.name == collection_name){
                        isFound = true;
                        cout<<"\033[1;31mCollection with same name already exists.\033[0m \n"<<endl;
                        break;
                    }
                }
                if(isFound == false){
                    createSchema(collection_name);
                }
            }
        }
        else if(query == "delete"){

        }
        else if(query == "search"){

        }
        else if(query == "exit"){
            cout<<"Nanda DB closing..."<<endl;
            break;
        }
        else{
            cout<<"Invalid command, type '--help' for information about code\n"<<endl;
        }
    }
    
}

int main() {
    Db system("system");
    Db massifcoder("massifcoder");
    databases["system"]=system;
    databases["massifcoder"]=massifcoder;
    demo();

    return 0;
}