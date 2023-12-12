#include <iostream>
using namespace std;

struct SSTNode {
    int id;
    char** key;

    SSTNode(int nodeId, char** nodeKey) : id(nodeId), key(nodeKey) {}

    ~SSTNode() {
        cout<<"Destructor running."<<endl;
        delete[] key;
    }
};

int main() {
    // Example usage
    char* key1 = new char[5]{'h', 'e', 'l', 'l', 'o'};
    char* key2 = new char[5]{'w', 'o', 'r', 'l', 'd'};

    char** keys = new char*[2]{key1, key2};

    SSTNode node(1, keys);

    // Accessing the object
    std::cout << "Node ID: " << node.id << std::endl;
    std::cout << "Node Key: " << *node.key[0] << *node.key[1] << std::endl;

    // Clean up

    return 0;
}
