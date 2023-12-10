#include<bits/stdc++.h>
using namespace std;

// Define a structure to represent a field in the table schema
struct Field {
    char name[32];
    char type[32];
    size_t size;

    Field(const std::string& fieldName, const std::string& fieldType)
        : size(getSizeOfType(fieldType)) {
            strncpy(this->name,fieldName.c_str(), sizeof(this->name)-1);
            this->name[sizeof(this->name) - 1] = '\0';
            strncpy(this->type,fieldType.c_str(), sizeof(this->type)-1);
            this->type[sizeof(this->type) - 1] = '\0';
        }

    size_t getSizeOfType(const std::string& fieldType) const {
        // Define your logic to determine the size based on the type
        // For simplicity, this example assumes fixed sizes for some types
        std::unordered_map<std::string, size_t> typeSize{
            {"int", sizeof(int)},
            {"double", sizeof(double)},
            {"string", sizeof(std::string)}
            // Add more types as needed
        };

        auto it = typeSize.find(fieldType);
        if (it != typeSize.end()) {
            return it->second;
        } else {
            // Default size for unknown types
            return 0;
        }
    }

};

// Define a structure to represent the table schema
struct TableSchema {
    std::string tableName;
    std::vector<Field> fields;
};

// Define a structure to represent metadata for a database
struct DatabaseMetadata {
    std::unordered_map<std::string, TableSchema> tables;
};

// Function to add a table to the database metadata
void addTable(DatabaseMetadata& metadata, const TableSchema& table) {
    metadata.tables[table.tableName] = table;
}

// Function to print the metadata of a database
void printMetadata(const DatabaseMetadata& metadata) {
    std::cout << "Database Metadata:\n";
    for (const auto& [tableName, table] : metadata.tables) {
        std::cout << "Table: " << table.tableName << "\n";
        std::cout << "Fields:\n";
        for (const auto& field : table.fields) {
            std::cout << "  Name: " << field.name << ", Type: " << field.type << ", Size: " << field.size << "\n";
        }
        std::cout << "\n";
    }
}

int main() {
    DatabaseMetadata myDatabase;

    // Define a schema for a user-defined table
    TableSchema userTable;
    cout<<"Enter the name of Table."<<endl;
    string nameTable;
    cin>>nameTable;
    userTable.tableName = nameTable;

    int number_of_fields;
    cout<<"Enter the number of fields you want to enter: ";
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

    // Print the metadata of the database
    printMetadata(myDatabase);

    return 0;
}
