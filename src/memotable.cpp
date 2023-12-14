#include "memotable.h"

IndexNode::IndexNode(int _id, long long _location, size_t _structSize) : id(_id), location(_location), structSize(_structSize) {}

IndexNode::IndexNode() {}

Node::Node() : id(0), height(0), isDeleted(false), left(nullptr), right(nullptr) {}

Node::Node(int key, string &pairsVal, bool isDeleted)
    : id(key), height(1), pairs(pairsVal), isDeleted(isDeleted), left(nullptr), right(nullptr) {}

int AVLTree::getHeight(Node *node)
{
    return (node) ? node->height : 0;
}

int AVLTree::getBalanceFactor(Node *node)
{
    return (node) ? getHeight(node->left) - getHeight(node->right) : 0;
}

Node *AVLTree::rotateRight(Node *node)
{
    Node *x = node->left;
    Node *y = x->right;

    x->right = node;
    node->left = y;

    node->height = max(getHeight(node->right), getHeight(node->left)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

Node *AVLTree::rotateLeft(Node *node)
{
    Node *x = node->right;
    Node *y = x->left;
    x->left = node;
    node->right = y;
    node->height = max(getHeight(node->right), getHeight(node->left)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

Node *AVLTree::update(Node *node, int key, string &pairs)
{
    if (node == nullptr)
    {
        number_of_nodes++;
        return new Node(key, pairs, false);
    }
    else if (node->id < key)
    {
        node->right = update(node->right, key, pairs);
    }
    else if (node->id > key)
    {
        node->left = update(node->left, key, pairs);
    }
    else
    {
        if(node->isDeleted == false){
            node->pairs = pairs;
        }
        else{
            return nullptr;
        }
    }
    return node;
}


Node *AVLTree::deleteNode(Node *node, int key, bool &isDeleted)
{
    if (node == nullptr)
    {
        return nullptr;
    }
    else if (node->id < key)
    {
        node->right = deleteNode(node->right, key, isDeleted);
    }
    else if (node->id > key)
    {
        node->left = deleteNode(node->left, key, isDeleted);
    }
    else
    {
        if (node->left == nullptr && node->right == nullptr)
        {
            delete node;
            number_of_nodes--;
            isDeleted = true;
            return nullptr;
        }
        else if (node->left == nullptr)
        {
            Node *temp = node->right;
            delete node;
            number_of_nodes--;
            isDeleted = true;
            return temp;
        }
        else if (node->right == nullptr)
        {
            Node *temp = node->left;
            isDeleted = true;
            delete node;
            number_of_nodes--;
            return temp;
        }
        else
        {
            Node *temp = node->right;
            while (temp->left != nullptr)
            {
                temp = temp->left;
            }
            node->id = temp->id;
            node->pairs = temp->pairs;
            node->right = deleteNode(node->right, temp->id, isDeleted);
        }
    }
    return node;
}

bool AVLTree::deleteNode(int id){
    bool isDeleted = false;
    root = deleteNode(root, id, isDeleted);
    return isDeleted;
}


Node *AVLTree::search(Node *node, int key)
{
    if (!node){
        return nullptr;
    }
    else if (key == node->id){
        return node;
    }
    else if (key < node->id){
        return search(node->left, key);
    }
    else{
        return search(node->right, key);
    }
}

Node *AVLTree::insert(Node *node, int key, string &pairs, bool &isInserted, bool isDeleted)
{
    if (!node){
        isInserted = true;
        number_of_nodes++;
        return new Node(key, pairs, isDeleted);
    }

    if (key < node->id){
        node->left = insert(node->left, key, pairs, isInserted);
    }
    else if (key > node->id){
        node->right = insert(node->right, key, pairs, isInserted);
    }
    else{
        if(node->isDeleted == true){
            node->isDeleted = false;
            node->pairs = pairs;
            isInserted = true;
            return node;
        }
        else{
            isInserted = false;
            return node;
        }
    }

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int balance = getBalanceFactor(node);

    if (balance > 1 && key < node->left->id){
        if (node->left != nullptr){
            return rotateRight(node);
        }
    }

    if (balance < -1 && key > node->right->id){
        if (node->right != nullptr){
            return rotateLeft(node);
        }
    }

    return node;
}

bool AVLTree::insert(string file_name, string index_file_name ,int id, string &pairs, bool isDeleted){
    bool isInserted = false;
    if(number_of_nodes == 3){
        flush_to_sst(file_name, index_file_name);
        number_of_nodes = 1;
        root = new Node(id, pairs, isDeleted);
        return true;
    }
    else{
        root = insert(root, id, pairs, isInserted, isDeleted);
        return isInserted;
    }
}

void AVLTree::printInOrder(Node *node){
    if (node){
        printInOrder(node->left);
        cout << "Person with id " << node->id << " has ";
        cout << node->pairs <<" and its deletion status is "<<node->isDeleted<< endl;
        printInOrder(node->right);
    }
}

void AVLTree::printInOrder(){
    if(this){
        cout<<"Number of nodes in the tree are "<<number_of_nodes<<endl;
    }
    else{
        cout<<"Tree is empty"<<endl;
    }

    printInOrder(this->root);
}

void AVLTree::write_to_sst(vector<pair<int, string>>&result, string file_name, string index_file_name){
    ofstream data_out(file_name, ios::binary);
    ofstream index_out(index_file_name, ios::binary);

    if(!data_out.is_open() || !index_out.is_open()){
        cout<<"Error in opening SST file. Check file location and error log file."<<endl;
        return ;
    }

    for(auto &node : result){
        int size = node.second.size() + 1;
        char data[size];
        strcpy(data, node.second.c_str());
        data[size-1] = '\0';
        long long location = data_out.tellp();
        data_out.write((char*)(&data), size);
        long long endLocation = data_out.tellp();
        IndexNode indexNode(node.first, location, size_t(endLocation - location));
        index_out.write((char*)&indexNode, sizeof(indexNode));
    }
    data_out.close();
    index_out.close();
}

void AVLTree::create_index_tree(vector<pair<int, string>>&result){ 
    if(root == nullptr){
        return;
    }

    queue<Node*> q;
    q.push(root);
    while(!q.empty()){

        Node* node = q.front();
        q.pop();
        if(node->isDeleted == false){
            result.push_back({node->id ,node->pairs});
        }
        else{
            result.push_back({node->id, ""});
        }
        if(node->left != nullptr){
            q.push(node->left);

        }

        if(node->right != nullptr){
            q.push(node->right);
        }

        delete(node);

    }
}

void AVLTree::write_index_tree(vector<pair<int,string>>&result, string file_name, string index_file_name){
    create_index_tree(result);
    write_to_sst(result, file_name, index_file_name);
}

void AVLTree::flush_to_sst(string file_name, string index_file_name){
    vector<pair<int,string>> result;
    write_index_tree(result, file_name, index_file_name);
    this->root = nullptr;
    this->number_of_nodes = 0;
    cout<<"Number of nodes are: "<<number_of_nodes<<endl;
    cout<<"Data flushed out!"<<endl;
}