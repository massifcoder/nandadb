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
        for (auto collection :database.second.collections){
            string collection_path = database_path + "/" + collection.second.name;
            if(!fs::exists(collection_path)){
                return false;
            }
            for(auto level : collection.second.levels){
                string level_path = collection_path + "/" + level.name + ".sst";
                string index_path = collection_path + "/" + level.name + ".idx";
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
    for(auto db : j["databasses"]){
        Db temp(db["name"]);
        for(auto collection : db["collections"]){
            vector<Level> levels;
            for(auto level : collection["levels"]){
                Level temp1(level["name"], level["numbers"]);
                levels.push_back(temp1);
            }
            Collection temp2(collection["name"], levels);
            temp.collections[collection["name"]] = temp2;
        }
        NandaDB.Databases[db["name"]] = temp;
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


json convertToJSON(const nandaDB& NandaDB){
    json j;
    j["name"] = NandaDB.name;
    for(const auto& database : NandaDB.Databases){
        json dbJson;
        dbJson["name"] = database.second.name;
        for(const auto& collection : database.second.collections){
            json collectionJson;
            collectionJson["name"] = collection.second.name;
            for(const auto& level : collection.second.levels){
                json levelJson;
                levelJson["name"] = level.name;
                levelJson["number"] = level.numbers;
                collectionJson["levels"].push_back(levelJson);
            }
            dbJson["collections"].push_back(collectionJson);
        }
        j["databasses"].push_back(dbJson);
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
        std::ofstream file(directory_path + "/" + level.name + ".sst");
        file.close();
        std::ofstream index_file(directory_path + "/" + level.name + ".idx");
        index_file.close();
    }
}

void insertValue(int id, string pairs, AVLTree &avl){
    bool isInserted = avl.insert(id, pairs);
    cout<<"Inside insert"<<endl;
    if(isInserted == false){
        cout<<"Such id is already present."<<endl;
    }
    else{
        avl.bitMap.set(id%10000);
        cout<<"Inserted successfully."<<endl;
    }
}

void deleteValue(int id, AVLTree &avl){
    string pairs = "";
    avl.insert(id, pairs, true);
    avl.bitMap.set(id%10000);
    cout<<"Deleted successfully."<<endl;
}

void updateValue(int id, string &pairs, AVLTree &avl){
    avl.insert(id, pairs);
    avl.bitMap.set(id%10000);
    cout<<"Values updated successfully."<<endl;
}

Node* searchValue(int id, AVLTree &avl){
    if(avl.bitMap.test(id%10000)){
       Node* ans = avl.search(avl.root, id);
       if(ans != nullptr){
            if(ans->isDeleted == true){
                return nullptr;
            }
            else{
                return ans;
            }
       } 
    }
    // if(sst1.bitMap.test(id%10000)){
        // Search SST_LV_1 -> will do tomorrow.
    // }
    // if(sst2.bitMap.test(id%10000)){
        // Search SST_LV_2 -> for future work.
    // }
    // if(sst3.bitMap.test(id%10000)){
        // Search SST_LV_3 -> for future work.
    // }
    // if(sst4.bitMap.test(id%10000)){
        // Search SST_LV_4 -> for futur work.
    // }
    return nullptr;
}

void insideCollection(string usermode, string collection_name){
    string query;
    AVLTree avl = NandaDB.Databases[usermode].collections[collection_name].avl;
    while(1){
        cout << "\033[0;32m" << usermode << " : " << collection_name << " :~ \033[1;34m";
        getline(cin, query);
        cout << "\033[0m";
        if (query == "--help"){
            help();
        }
        else if (query == "clear"){
            clearScreen();
        }
        else if (query.substr(0, 13) == "insert value "){
            try{
                auto start = chrono::high_resolution_clock::now();
                string bson = query.substr(13, query.size() - 13);
                json j = json::parse(bson);
                int id = j["id"].get<int>();
                string pairs = j["pairs"].dump();
                cout<<id<<" "<<pairs<<endl;
                if(!avl.bitMap.test(id%10000)){//} && !sst1.bitMap.test(id%10000) && !sst2.bitMap.test(id%10000 && !sst3.bitMap.test(id%10000)) && !sst4.bitMap.test(id%10000)){
                    insertValue(id, pairs, avl);
                }
                else{
                    Node*ans = searchValue(id, avl);
                    if(ans == nullptr){
                        insertValue(id, pairs, avl);
                    }
                    else{
                        cout<<"Such id is already present."<<endl;
                    }
                }
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                cout << "Duration: " << duration.count() << " microseconds" << endl;
            }
            catch(const std::exception& e){
                cout<<"Invalid bson format."<<endl;
            }
        }
        else if (query.substr(0, 13) == "delete value "){
            try{
                auto start = chrono::high_resolution_clock::now();
                string json_string = query.substr(13, query.size() - 13);
                json j = json::parse(json_string);
                int id = j["id"].get<int>();
                cout<<id<<endl;
                bool isDeleted = avl.deleteNode(id);
                if(isDeleted == false){
                    deleteValue(id, avl);
                }
                else{
                    cout<<"Deleted successfully."<<endl;
                }
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                cout << "Duration: " << duration.count() << " microseconds" << endl;
            }
            catch(const std::exception& e){
                cout<<"Invalid bson format."<<endl;
            }
        }
        else if(query == "select *"){
            try{
                avl.printInOrder();
            }
            catch(const std::exception& e){
                cout<<"Invalid format."<<endl;
            }
        }
        else if(query.substr(0, 13) == "select value "){
            try{
                auto start = chrono::high_resolution_clock::now();
                string json_string = query.substr(13, query.size() - 13);
                json j = json::parse(json_string);
                int id = j["id"].get<int>();
                Node* ans = searchValue(id, avl);
                if(ans == nullptr){
                    cout<<"No such id present."<<endl;
                }
                else{
                    cout<<"Id: "<<ans->id<<" and bson: "<<ans->pairs<<endl;
                }
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                cout << "Duration: " << duration.count() << " microseconds" << endl;
            }
            catch(const std::exception& e){
                cout<<"Invalid bson format."<<endl;
            }
        }
        else if(query.substr(0, 13) == "update value "){
            try{
                auto start = chrono::high_resolution_clock::now();
                string json_string = query.substr(13, query.size() - 13);
                json j = json::parse(json_string);
                int id = j["id"].get<int>();
                string pairs = j["pairs"].dump();
                Node* ans = avl.update(avl.root, id, pairs);
                if(ans == nullptr){
                    updateValue(id, pairs, avl);
                }
                else{
                    cout<<"Updated successfully. With id "<<ans->id<<" and bson of "<<ans->pairs<<endl;
                }
                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                cout << "Duration: " << duration.count() << " microseconds" << endl;
            }
            catch(const std::exception& e){
                cout<<"Invalid bson format."<<endl;
            }
        }
        else if(query.substr() == "exit collection"){
            cout << "\033[1;31mCollection closed successfully.\033[0m \n" << endl;
            cout << endl;
            return;
        }
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
        else if(query.substr(0, 15) == "use collection "){
            string collection_name = query.substr(15, query.size() - 15);
            if(usermode == NandaDB.name){
                cout << "\033[1;31mSelect a database where you want to work.\033[0m \n"<< endl;
            }
            else{
                if(NandaDB.Databases.find(usermode) == NandaDB.Databases.end()){
                    cout << "\033[1;31mNo such database present.\033[0m" <<endl;
                }
                else{
                    if(NandaDB.Databases[usermode].collections[collection_name].name == collection_name){
                        cout << "\033[1;31mCollection selected successfully.\033[0m \n" << endl;
                        insideCollection(usermode, collection_name);
                    }
                    else{
                        cout << "\033[1;31mNo such collection present.\033[0m" << endl;
                    }
                }
            }
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
                    for(auto collection : NandaDB.Databases[usermode].collections){
                        cout<<" > "<<collection.first<<endl;
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
                if(NandaDB.Databases[usermode].collections.find(collection_name) != NandaDB.Databases[usermode].collections.end()){
                    cout << "\033[1;31mCollection with same name already exists.\033[0m \n"<< endl;
                }
                else{
                    fs::path base_path = fs::current_path();
                    string directory_path = string(base_path) + "/database/" + usermode + "/" + collection_name;
                    fs::create_directory(directory_path);
                    vector<Level> levels{
                        Level({"SST_LV_1", 0}),
                        Level({"SST_LV_2", 0}),
                        Level({"SST_LV_3", 0}),
                        Level({"SST_LV_4", 0})
                    };
                    Collection to_ad = Collection(collection_name, levels );
                    NandaDB.Databases[usermode].collections[collection_name] = to_ad;
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
                if(NandaDB.Databases[usermode].collections.find(collection_name) == NandaDB.Databases[usermode].collections.end()){
                    cout << "\033[1;31mNo such collection present.\033[0m" << endl;
                }
                else{
                    fs::path base_path = fs::current_path();
                    string directory_path = string(base_path) + "/database/" + usermode + "/" + collection_name;
                    fs::remove_all(directory_path);
                    NandaDB.Databases[usermode].collections.erase(collection_name);
                    saveMetadata();
                    cout << "\033[1;31mCollection deleted successfully.\033[0m \n"<< endl;
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