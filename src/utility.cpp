#include "utility.h"

// // Global variable.
// AVLTree avl;
// int MAX_NODE = 262143;


// void printInOrder(){
//     avl.printInOrder();
// }

// void create_index_tree(vector<pair<int,string>>&result){
    
//     if(avl.root == nullptr){
//         return;
//     }

//     queue<Node*> q;
//     q.push(avl.root);
//     while(!q.empty()){

//         Node* node = q.front();
//         q.pop();
//         result.push_back({node->id ,node->pairs});
//         if(node->left != nullptr){
//             q.push(node->left);

//         }

//         if(node->right != nullptr){
//             q.push(node->right);
//         }

//         delete(node);

//     }
// }

// // Write to disk.
// void write_to_sst(vector<pair<int,string>>&result){

//     ofstream data_out("SST_LV_1/sst_1.lv_1", ios::binary);
//     ofstream index_out("SST_LV_1/index_1.lv_1", ios::binary);

//     if(!data_out.is_open() || !index_out.is_open()){
//         cout<<"File is not opening, some errors in file system, run some debugging methods."<<endl;
//     }

//     for(auto node : result){
//         int size = node.second.size()+1;
//         char data[size];
//         strcpy(data, node.second.c_str());
//         data[size] = '\0';
//         long long location = data_out.tellp();
//         data_out.write((char*)(&data), size);
//         long long endLocation = data_out.tellp();
//         IndexNode indexNode(node.first, location, size_t(endLocation-location));
//         index_out.write((char*)&indexNode, sizeof(indexNode));
//     }

//     data_out.close();
//     index_out.close();

// }

// // Write with primary key.
// void write_index_tree(vector<pair<int,string>>&result){
//     create_index_tree(result);
//     write_to_sst(result);
// }

// // Flushing data from main memory to disk.
// void flush_to_sst(){
//     vector<pair<int,string>> result;
//     write_index_tree(result);
//     avl.root = nullptr;
//     cout<<"Data flushed out!"<<endl;
// }

