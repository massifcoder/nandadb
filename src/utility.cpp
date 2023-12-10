#include "utility.h"

// Global variable.
AVLTree avl;

// Creation of AVL Tree.
void creationAVL(){
    avl.insert(1, "John", 25, 100, "1234567890");
    avl.insert(2, "Jane", 30, 150, "9876543210");
    avl.insert(3, "Bob", 22, 80, "5555555555");
}

// Printing AVL Tree using in-order traversal.
void printInOrder(){
    avl.printInOrder();
}

// Taking data as vector in pre-order traversal.
void create_index_tree(vector<SSTNode>&result){
    
    if(avl.root == nullptr){
        return;
    }

    queue<Node*> q;
    q.push(avl.root);
    while(!q.empty()){

        Node* node = q.front();
        q.pop();
        result.push_back(SSTNode(node->id, node->name, node->age, node->purchased_value, node->contact_number));
        
        if(node->left != nullptr){
            q.push(node->left);

        }

        if(node->right != nullptr){
            q.push(node->right);
        }

        delete(node);

    }
}

// Write to disk.
void write_to_sst(vector<SSTNode>&result){

    ofstream data_out("SST_LV_1/sst_1.lv_1", ios::binary);
    ofstream index_out("SST_LV_1/index_1.lv_1", ios::binary);

    if(!data_out.is_open() || !index_out.is_open()){
        cout<<"File is not opening, some errors in file system, run some debugging methods."<<endl;
    }

    for(auto node : result){
        // Writing data to file.
        data_out.write((char*)&node, sizeof(node));

        // Getting the current position of data in file.
        long long location = data_out.tellp();
        // SSTNode indexNode = {node.id, location};


        // Storing the current position in index table.
        index_out.write((char*)&location, sizeof(location));

    }

    data_out.close();
    index_out.close();

}

// Write with primary key.
void write_index_tree(vector<SSTNode>&result){
    create_index_tree(result);

    write_to_sst(result);

}

// Write with 2nd primary key.
void write_index_b_tree(vector<SSTNode>&result){
    create_index_tree(result);
}

// Flushing data from main memory to disk.
void flush_to_sst(){
    bool second_primary = false;
    vector<SSTNode> result;

    if(second_primary){
        write_index_b_tree(result);
    }
    else{
        write_index_tree(result);
    }

    avl.root = nullptr;
    // write_to_sst(result);

    cout<<"Data flushed out!"<<endl;
}

void load_from_sst(){
    // Assumptions :
    // 1 > Bloom filters give hint.
    // 2 > Memory Management system (semaphores) cleared the assurance.
    // 3 > If p1_key given, then use index table, if p2_key then use file table, if not then iterate.
    // 4 > Add caching system with multilevel aging system.

}