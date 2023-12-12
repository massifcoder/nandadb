#include "utility.h"

// Global variable.
AVLTree avl;
int MAX_NODE = 1023;


void printInOrder(){
    avl.printInOrder();
}

string serialize(Node* node){
    string serialised = "id:";
    serialised += to_string(node->id) + ",";
    for(int i=0; i<node->pairs.size(); i++){
        serialised += node->pairs[i].first + ":";
        serialised += node->pairs[i].second + ",";
    }
    return serialised;
}

void create_index_tree(vector<pair<int,string>>&result){
    
    if(avl.root == nullptr){
        return;
    }

    queue<Node*> q;
    q.push(avl.root);
    while(!q.empty()){

        Node* node = q.front();
        q.pop();
        string serl = serialize(node);
        cout<<"Size is :"<<serl.size()<<endl;
        result.push_back({node->id ,serl});
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
void write_to_sst(vector<pair<int,string>>&result){

    ofstream data_out("SST_LV_1/sst_1.lv_1", ios::binary);
    ofstream index_out("SST_LV_1/index_1.lv_1", ios::binary);

    if(!data_out.is_open() || !index_out.is_open()){
        cout<<"File is not opening, some errors in file system, run some debugging methods."<<endl;
    }

    for(auto node : result){
        int size = node.second.size()+1;
        char data[size];
        strcpy(data, node.second.c_str());
        data[size] = '\0';
        long long location = data_out.tellp();
        data_out.write((char*)(&data), size);
        long long endLocation = data_out.tellp();
        IndexNode indexNode(node.first, location, size_t(endLocation-location));
        index_out.write((char*)&indexNode, sizeof(indexNode));
    }

    data_out.close();
    index_out.close();

}

// Write with primary key.
void write_index_tree(vector<pair<int,string>>&result){
    create_index_tree(result);
    write_to_sst(result);
}

// Flushing data from main memory to disk.
void flush_to_sst(){
    vector<pair<int,string>> result;
    write_index_tree(result);
    avl.root = nullptr;
    cout<<"Data flushed out!"<<endl;
}

pair<long long, size_t> find_location_by_key(int search_key){
    ifstream index_in("SST_LV_1/index_1.lv_1", ios::binary);

    if(!index_in.is_open()){
        cout<<"Error in opening index file."<<endl;
        return {-1,-1};
    }

    IndexNode indexNode;
    long long current_position = 0;

    while(index_in.read((char*)&indexNode, sizeof(indexNode))){
        int val = indexNode.id;
        if(indexNode.id == search_key){
            index_in.close();
            return {indexNode.location, indexNode.structSize};
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
    return {-1, -1};

}

void deserialize(string serialised, Node& dataNode){
    int i=0;
    string key="";
    string value="";
    while(i<serialised.size()){
        if(serialised[i] == ':'){
            key = value;
            value = "";
        }
        else if(serialised[i] == ','){
            dataNode.pairs.push_back({key, value});
            key = "";
            value = "";
        }
        else{
            value += serialised[i];
        }
        i++;
    }
}

void read_data_from_location(long long location, size_t structSize){
    ifstream data_in("SST_LV_1/sst_1.lv_1");

    if(!data_in.is_open()){
        cout<<"Error in opening SST file. Check file location and error log file."<<endl;
        return ;
    }

    data_in.seekg(location);
    if (!data_in.is_open()) {
        cout<<"Error"<<endl;
    }
    char serialised[structSize];
    cout<<structSize<<endl;
    data_in.read((char*)&serialised, structSize);
    cout<<"Serialed data is:";
    cout<<serialised<<endl;

    Node dataNode;
    deserialize(serialised, dataNode);

    cout<<"Data is: "<<endl;
    for(auto pair : dataNode.pairs){
        cout<<pair.first<<" : "<<pair.second<<endl;
    }

    data_in.close();

}

void load_from_sst(){
    pair<long long, size_t> info = find_location_by_key(3);
    if(info.first != -1){
        cout<<"Struct is at location: "<<info.first<<endl;
        read_data_from_location(info.first, info.second);
    }
    else{
        cout<<"Data not present with this key id."<<endl;
    }
}