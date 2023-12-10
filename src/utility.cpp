#include "utility.h"

// Global variable.
AVLTree avl;
int MAX_NODE = 1023;

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
        long long location = data_out.tellp();
        data_out.write((char*)&node, sizeof(node));
        IndexNode indexNode(node.id, location);
        index_out.write((char*)&indexNode, sizeof(indexNode));
    }

    data_out.close();
    index_out.close();

}

// Write with primary key.
void write_index_tree(vector<SSTNode>&result){
    create_index_tree(result);
    write_to_sst(result);
}


// Flushing data from main memory to disk.
void flush_to_sst(){
    bool second_primary = false;
    vector<SSTNode> result;
    write_index_tree(result);
    avl.root = nullptr;
    cout<<"Data flushed out!"<<endl;
}

long long find_location_by_key(int search_key){
    ifstream index_in("SST_LV_1/index_1.lv_1", ios::binary);

    if(!index_in.is_open()){
        cout<<"Error in opening index file."<<endl;
        return -1;
    }

    IndexNode indexNode;
    long long current_position = 0;

    while(index_in.read((char*)&indexNode, sizeof(indexNode))){
        int val = indexNode.id;
        cout<<val<<endl;
        if(indexNode.id == search_key){
            index_in.close();
            return indexNode.location;
        }
        else if(indexNode.id > search_key){
            current_position = 2 * current_position + 1;
        }
        else{
            current_position = 2 * current_position + 2;
        }

        index_in.seekg(current_position * sizeof(indexNode), ios::beg);
    
    }

    index_in.close();
    return -1;

}

void read_data_from_location(long long location){
    ifstream data_in("SST_LV_1/sst_1.lv_1");

    if(!data_in.is_open()){
        cout<<"Error in opening SST file. Check file location and error log file."<<endl;
        return ;
    }

    SSTNode dataNode;
    data_in.seekg(location);

    data_in.read((char*)&dataNode, sizeof(dataNode));
    
    cout <<"Person with id: "<< dataNode.id<<" has name of "<<dataNode.name<< " with "<<dataNode.contact_number << " has purchased value of " << dataNode.purchased_value << endl;
    data_in.close();

}

void load_from_sst(){
    long long location = find_location_by_key(1);
    if(location != -1){
        read_data_from_location(location);
    }
    else{
        cout<<"Data not present with this key id."<<endl;
    }
}