#include "field.h"

DatabaseMetadata myDatabase;

Field::Field(const std::string &fieldName, const std::string &fieldType){
    strncpy(this->name, fieldName.c_str(), sizeof(this->name) - 1);
    this->size = getSizeOfType(fieldType);
    this->name[sizeof(this->name) - 1] = '\0';
    strncpy(this->type, fieldType.c_str(), sizeof(this->type) - 1);
    this->type[sizeof(this->type) - 1] = '\0';
}

size_t Field::getSizeOfType(const std::string &fieldType){
    std::unordered_map<std::string, size_t> typeSize{
        {"int", sizeof(int)},
        {"double", sizeof(double)},
        {"string", sizeof(std::string)}
    };

    auto it = typeSize.find(fieldType);
    if (it != typeSize.end()){
        return it->second;
    }
    else{
        return 0;
    }
}

void addTable(DatabaseMetadata &metadata, const TableSchema &table){
    metadata.tables[table.tableName] = table;
    fs::path base_path = fs::current_path();
    string collection_directory = string(base_path) + "/database/massifcoder/" + table.tableName;
    if(!fs::exists(collection_directory)){
        fs::create_directory(collection_directory);
    } 
    collection_directory += "/metadata";
    ofstream meta_data_out(collection_directory);
    string data_to_write = "name:"+table.tableName;
    meta_data_out << data_to_write;
    vector<Field> collection = table.fields;
    data_to_write = "fields:"+ table.tableName.size();
    meta_data_out << data_to_write;
    for(auto field : collection){
        data_to_write = "name:" + string(field.name);
        meta_data_out << data_to_write;
        data_to_write = "type:" + string(field.type);
        meta_data_out << data_to_write;
        data_to_write = "size:" + field.size;
        meta_data_out << data_to_write;
    }
}

void printMetadata(const DatabaseMetadata &metadata){
    cout << "Database Metadata:\n";
    for (const auto &[tableName, table] : metadata.tables){
        std::cout << "Table: " << table.tableName << "\n";
        std::cout << "Fields:\n";
        for (const auto &field : table.fields){
            std::cout << "  Name: " << field.name << ", Type: " << field.type << ", Size: " << field.size << "\n";
        }
        std::cout << "\n";
    }
}

void createSchema(string nameTable){
    TableSchema userTable;
    userTable.tableName = nameTable;

    int number_of_fields;
    cout<<"\nEnter the number of fields you want to enter: ";
    cin>>number_of_fields;

    string field_name;
    string field_type;
    for(int i=0; i<number_of_fields; i++){
        cout<<"Enter the filed name: ";
        cin>>field_name;
        cout<<"Enter the field type: ";
        cin>>field_type;
        userTable.fields.push_back({field_name, field_type});
    }

    // Add the user-defined table to the database metadata
    addTable(myDatabase, userTable);
    cout<<endl;
    // Print the metadata of the database
    printMetadata(myDatabase);

    return;
}