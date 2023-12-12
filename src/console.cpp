#include "console.h"

bool passed_post = false;

void POST(){
    cout<<"Power On Self Test."<<endl; 
    cout<<"Hold..."<<endl;
    cout<<endl;

    ifstream f("metadata.db");
    if(!f.good()){
        cout<<"System Crashed!"<<endl;
    }
    else{
        cout<<"Loading Nanda DB..."<<endl;
        cout<<endl;
    }

    ifstream i("metadata.db");
    json j;
    i>>j;
    i.close();

    NandaDB.name = j["name"];
    for(auto db : j["Databases"]){
        Db temp(db["name"]);
        for(auto collection : db["collections"]){
            Collection temp2(collection["name"]);
            temp.collections.push_back(temp2);
        }
        NandaDB.Databases[temp.name] = temp;
    }

    cout<<NandaDB.name<<" loaded."<<endl;
    cout<<endl;
    for(auto db : NandaDB.Databases){
        cout<<db.first<<" loaded."<<endl;
        for(auto collection : db.second.collections){
            cout<<collection.name<<" loaded."<<endl;
        }
        cout<<endl;
    }

    cout<<"Nanda DB loaded."<<endl;
    cout<<endl;
    cout<<"Nanda DB is ready to use."<<endl;
    cout<<endl;
    cout<<"Press any key to continue..."<<endl;
    getch();
    passed_post = true;
}

bool verify()
{
    string name, password;
    cout << "Enter your username." << endl;
    cin >> name;
    cout << "Enter your password" << endl;
    cin >> password;
    if (username == name && password == pwd)
    {
        cout << "Welcome to Nanda DB.\n"
             << endl;
        return 1;
    }
    else
    {
        cout << "Wrong credentials" << endl;
        return 0;
    }
}

void clearScreen()
{
    cout << "\033[2J\033[H";
}

void help()
{
    cout << "\n\033[1;31mTo find docs, type '--help', to insert value in database type 'add', to search value type 'search' and to delete type 'delete'. To exit the program type 'exit' and to clear screen 'clear'.\033[0m \n"
         << endl;
}

void Console(){
    if(!passed_post){
        cout<<"Failed POST test, means you have done something wrong with the structure."<<endl;
        return;
    }
    bool login = verify();
    if (!login)
    {
        cout << "You are failed to use our database." << endl;
        return;
    }
    string usermode = NandaDB.name;
    string query;
    help();
    while(true){
        cout << "\033[0;32m" << usermode << " :~ \033[1;34m";
        getline(cin, query);
        cout << "\033[0m";
        if (query == "--help")
        {
            help();
        }
        else if (query == "clear")
        {
            clearScreen();
        }
        else if (query == "show databases")
        {
            usermode = "Nanda DB";
            cout << "\033[1;31mList of databases:\033[0m" << endl;
            for (auto database : NandaDB.Databases)
            {
                cout << " > " << database.first << endl;
            }
            cout << endl;
        }
        else if (query.substr(0, 4) == "use ")
        {
            string database = query.substr(4, query.size() - 4);
            if (database == "system")
            {
                cout << "\033[1;31mYou have no permission! Its for internal working.\033[0m" << endl;
            }
            else if (NandaDB.Databases.find(database) == NandaDB.Databases.end())
            {
                cout << "\033[1;31mNo such database present.\033[0m" << endl;
            }
            else
            {
                usermode = database;
                cout << endl;
            }
        }
        else if (query == "add")
        {
            auto start = chrono::high_resolution_clock::now();
            // Start code

            // Code ends here and now vishal will analyze.
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
            cout << "Duration: " << duration.count() << " microseconds" << endl;
        }
        else if (query == "show collections")
        {
            if (usermode == "Nanda DB")
            {
                cout << "\033[1;31mSelect a database where you want to work.\033[0m \n"
                     << endl;
            }
            else
            {
                if (NandaDB.Databases.find(usermode) != NandaDB.Databases.end())
                {
                    cout << "\033[1;31mList of collections in " << usermode << " are :\033[0m" << endl;
                    vector<Collection> collections = NandaDB.Databases[usermode].collections;
                    for (auto &collection : collections)
                    {
                        cout << " > " << collection.name << endl;
                    }
                    cout << endl;
                }
            }
        }
        else if(query.substr(0, 16) == "create database "){
            string database_name = query.substr(16, query.size() - 16);
            cout << database_name << endl;
            if (database_name.size() < 4)
            {
                cout << "\033[1;31mName should be of size more than or equal to 4.\033[0m \n" << endl;
            }
            else if (NandaDB.Databases.find(database_name) != NandaDB.Databases.end())
            {
                cout << "\033[1;31mDatabase with same name already exists.\033[0m \n" << endl;
            }
            else
            {
                fs::path base_path = fs::current_path();
                fs::path directory_path = base_path / "database" / database_name;
                fs::create_directory(directory_path);
                NandaDB.Databases[database_name] = Db(database_name);
                cout << "\033[1;31mDatabase created successfully.\033[0m \n"<< endl;
            }
        }
        else if (query.substr(0, 18) == "create collection ")
        {
            string collection_name = query.substr(18, query.size() - 18);
            cout << collection_name << endl;
            if (collection_name.size() < 4)
            {
                cout << "\033[1;31mName should be of size more than or equal to 4.\033[0m \n"
                     << endl;
            }
            else if (usermode == "Nanda DB")
            {
                cout << "\033[1;31mSelect a database where you want to work.\033[0m \n"
                     << endl;
            }
            else if (NandaDB.Databases.find(usermode) == NandaDB.Databases.end())
            {
                cout << "\033[1;31mNo such database present.\033[0m" << endl;
            }
            else
            {
                vector<Collection> &collections = NandaDB.Databases[usermode].collections;
                bool isFound = false;
                for (auto &collection : collections)
                {
                    if (collection.name == collection_name)
                    {
                        isFound = true;
                        cout << "\033[1;31mCollection with same name already exists.\033[0m \n"<< endl;
                        break;
                    }
                }
                if (isFound == false)
                {
                    fs::path base_path = fs::current_path();
                    fs::path directory_path = base_path / "database" / usermode / collection_name;
                    fs::create_directory(directory_path);
                    NandaDB.Databases[usermode].collections.push_back(Collection(collection_name));
                    cout << "\033[1;31mCollection created successfully.\033[0m \n"<< endl;
                }
            }
        }
        else if (query == "delete")
        {
        }
        else if (query == "search")
        {
        }
        else if (query == "exit")
        {
            cout << "Nanda DB closing..." << endl;
            break;
        }
        else
        {
            cout << "Invalid command, type '--help' for information about code\n"<< endl;
        }
    }
}