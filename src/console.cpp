#include "console.h"

void clearScreen()
{
    cout << "\033[2J\033[H";
}

bool verify_database_file(){
    fs::path base_path = fs::current_path();
    string directory_path = string(base_path) + "/database";
    if(!fs::exists(directory_path)){
        return false;
    }
    for(auto database : NandaDB.Databases){
        string database_path = directory_path + "/" + database.first;
        if(!fs::exists(database_path)){
            return false;
        }
        for(auto collection : database.second.collections){
            string collection_path = database_path + "/" + collection.name;
            if(!fs::exists(collection_path)){
                return false;
            }
            for(auto level : collection.levels){
                string level_path = collection_path + "/" + level;
                if(!fs::exists(level_path)){
                    return false;
                }
            }
        }
    }
    return true;
}

bool performPOST(){
    cout<<"Power On Self Test."<<endl; 
    cout<<"Hold..."<<endl;
    cout<<endl;

    ifstream i("metadata.db");

    if(!i.is_open()){
        cout<<"System Crashed!"<<endl;
        return false;
    }

    json j;
    i>>j;
    i.close();

    NandaDB.name = j["name"];
    for(auto db : j["databases"]){
        Db temp(db["name"]);
        for(auto collection : db["collections"]){
            Collection temp2(collection["name"], collection["levels"]);
            temp.collections.push_back(temp2);
        }
        NandaDB.Databases[temp.name] = temp;
    }

    bool passed_post = verify_database_file();

    if(passed_post){
        cout<<"System is ready to use."<<endl;
        cout<<endl;
        cout<<"Press any key to continue..."<<endl;
        getchar();
        clearScreen();
        return true;
    }
    else{
        cout<<"System Crashed, due to missing directory files!"<<endl;
        return false;
    }

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


void help()
{
    cout << "\n\033[1;31mTo find docs, type '--help', to insert value in database type 'add', to search value type 'search' and to delete type 'delete'. To exit the program type 'exit' and to clear screen 'clear'.\033[0m \n"
         << endl;
}

json convertToJSON(const nandaDB& NandaDB) {
    json j;
    j["name"] = NandaDB.name;

    for (const auto& database : NandaDB.Databases) {
        json dbJson;
        dbJson["name"] = database.second.name;

        for (const auto& collection : database.second.collections) {
            json collectionJson;
            collectionJson["name"] = collection.name;
            collectionJson["levels"] = collection.levels;
            dbJson["collections"].push_back(collectionJson);
        }

        j["databases"].push_back(dbJson);
    }

    return j;
}

void saveMetadata() {
    json j = convertToJSON(NandaDB);
    ofstream o("metadata.db");
    o << setw(4) << j << endl;
    o.close();
}

void createFiles(const Collection& collection, string directory_path) {
    for (const auto& level : collection.levels) {
        std::ofstream file(directory_path + "/" + level + ".sst");
        file.close();
    }
}

void Console(){
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
            usermode = NandaDB.name;
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
        else if (query.substr(0,7) == "insert ")
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
            if (usermode == NandaDB.name)
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
                string directory_path = string(base_path) + "/database/" + database_name;
                fs::create_directory(directory_path);
                NandaDB.Databases[database_name] = Db(database_name);
                saveMetadata();
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
            else if (usermode == NandaDB.name)
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
                    string directory_path = string(base_path) + "/database/" + usermode + "/" + collection_name;
                    fs::create_directory(directory_path);
                    Collection to_ad = Collection(collection_name, {"memo", "SST_LV_1", "SST_LV_2"});
                    NandaDB.Databases[usermode].collections.push_back(to_ad);
                    createFiles(to_ad, directory_path);
                    saveMetadata();
                    cout << "\033[1;31mCollection created successfully.\033[0m \n"<< endl;
                }
            }
        }
        else if (query.substr(0, 16) == "delete database ")
        {
            string database_name = query.substr(16, query.size() - 16);
            cout << database_name << endl;
            if (database_name.size() < 4)
            {
                cout << "\033[1;31mName should be of size more than or equal to 4.\033[0m \n"<< endl;
            }
            else if (NandaDB.Databases.find(database_name) == NandaDB.Databases.end())
            {
                cout << "\033[1;31mNo such database present.\033[0m" << endl;
            }
            else
            {
                fs::path base_path = fs::current_path();
                string directory_path = string(base_path) + "/database/" + database_name;
                fs::remove_all(directory_path);
                NandaDB.Databases.erase(database_name);
                usermode = NandaDB.name;
                saveMetadata();
                cout << "\033[1;31mDatabase deleted successfully.\033[0m \n"<< endl;
            }
        }
        else if (query.substr(0, 18) == "delete collection ")
        {
            string collection_name = query.substr(18, query.size() - 18);
            cout << collection_name << endl;
            if (collection_name.size() < 4)
            {
                cout << "\033[1;31mName should be of size more than or equal to 4.\033[0m \n"<< endl;
            }
            else if (usermode == NandaDB.name)
            {
                cout << "\033[1;31mSelect a database where you want to work.\033[0m \n"<< endl;
            }
            else if (NandaDB.Databases.find(usermode) == NandaDB.Databases.end())
            {
                cout << "\033[1;31mNo such database present.\033[0m" << endl;
            }
            else
            {
                vector<Collection> &collections = NandaDB.Databases[usermode].collections;
                bool isFound = false;
                for (auto it = collections.begin(); it != collections.end(); ++it)
                {
                    if (it->name == collection_name)
                    {
                        isFound = true;
                        fs::path base_path = fs::current_path();
                        string directory_path = string(base_path) + "/database/" + usermode + "/" + collection_name;
                        fs::remove_all(directory_path);
                        collections.erase(it);
                        saveMetadata();
                        cout << "\033[1;31mCollection deleted successfully.\033[0m \n" << endl;
                        break;
                    }
                }
                if (isFound == false)
                {
                    cout << "\033[1;31mNo such collection present.\033[0m" << endl;
                }
            }
        }
        else if (query.substr(0, 7) == "delete ")
        {
            if(usermode == NandaDB.name){
                cout << "\033[1;31mSelect a database where you want to work.\033[0m \n"<< endl;
            }
            else if (NandaDB.Databases.find(usermode) == NandaDB.Databases.end()){
                cout << "\033[1;31mSelect a collection where you want to work.\033[0m \n"<< endl;
            }
            else{
                cout<<"Working"<<endl;
            }
        }
        else if (query.substr(0, 7) == "select ")
        {
            if(usermode == NandaDB.name){
                cout << "\033[1;31mSelect a database where you want to work.\033[0m \n"<< endl;
            }
            else if (NandaDB.Databases.find(usermode) == NandaDB.Databases.end()){
                cout << "\033[1;31mSelect a collection where you want to work.\033[0m \n"<< endl;
            }
            else{
                cout<<"Working"<<endl;
            }
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