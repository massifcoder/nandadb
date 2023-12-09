#include<bits/stdc++.h>
using namespace std;
#define limitSize 1000

struct SSTNode{
    int id;
    char name[20];
    int age;
    int purchased_value;
    char contact_number[10];

    SSTNode(int key, string name, int umar, int pur, string contact) : id(key), age(umar), purchased_value(pur){
        strcpy(this->name ,name.c_str());
        strcpy(this->contact_number, contact.c_str());
    }

};

struct Node{
    int id;
    string name;
    int age;
    int purchased_value;
    string contact_number;
    int height;
    Node* right;
    Node* left;

    Node(int key, string naam, int umar, int pur, string contact) : id(key), name(naam), age(umar), purchased_value(pur),contact_number(contact) , height(1), left(nullptr), right(nullptr) {}

};

class AVLTree{
    public:

        Node* root;

        int getHeight(Node* node){
            return (node) ? node->height : 0;
        }

        int getBalanceFactor(Node* node){
            return (node) ? getHeight(node->left) - getHeight(node->right) : 0;
        }

        Node* rotateRight(Node* node){
            Node* x = node->left;
            Node* y = x->right;

            x->right = node;
            node->left = y;
            
            node->height = max(getHeight(node->right), getHeight(node->left)) + 1;
            x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

            return x;
        }

        Node* rotateLeft(Node* node){
            Node* x = node->right;
            Node* y = x->left;
            x->left = node;
            node->right = y;
            node->height = max(getHeight(node->right), getHeight(node->left)) + 1;
            x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

            return x;
        }

        Node* insert(Node* node, int key, string name, int age, int purchased_value, string contact_number){
            if(!node){
                return new Node(key, name, age, purchased_value, contact_number);
            }

            if(key < node->id){
                node->left = insert(node->left, key, name, age, purchased_value, contact_number);
            }

            if(key > node->id){
                node->right = insert(node->right, key, name, age, purchased_value, contact_number);
            }

            else{
                return node;
            }

            node->height = 1 + max(getHeight(node->left), getHeight(node->right));
            int balance = getBalanceFactor(node);

            if(balance > 1 && key < node->left->id){
                return rotateRight(node);
            }

            if(balance < -1 && key > node->right->id){
                return rotateLeft(node);
            }

            return node;

        }

        void insert(int id, string name, int age, int purchased_value, string contact_number){
            this->root = insert(root, id, name, age, purchased_value, contact_number);
        }


};

void help(){
    cout<<"For adding value, type 'add'"<<endl<<endl;
}

void preOderTraversal(Node* node, vector<SSTNode>&result){
    if(node != nullptr){
        result.push_back(SSTNode(node->id,node->name,node->age,node->purchased_value,node->contact_number));
        delete(node);
        preOderTraversal(node->left, result);
        preOderTraversal(node->right, result);
    }
}

SSTNode searchInSSTable(int id){
    
}

void flush_data(AVLTree &avlTree){
    vector<SSTNode> result;
    preOderTraversal(avlTree.root, result);
    avlTree.root = nullptr;
    ofstream out("samp.txt");
    if(!out.is_open()){
        cout<<"Error in opening file."<<endl;
        return;
    }
    for(auto node : result){
        out.write((char*)&node, sizeof(node));
    }
    out.close();
}

void add(AVLTree &avlTree, int &index){
    int id, age, purchased_value;
    string name, contact_number;
    cout<<"Enter the id of the customer."<<endl;
    cin>>id;
    cout<<"Enter the name of the customer."<<endl;
    cin>>name;
    cout<<"Enter the age of the customer."<<endl;
    cin>>age;
    cout<<"Enter the value of purchasing item."<<endl;
    cin>>purchased_value;
    cout<<"Enter the contact number of the person."<<endl;
    cin>>contact_number;
    avlTree.insert(id, name, age, purchased_value,contact_number);
    index++;
    if(index >= limitSize){
        flush_data(avlTree);
    }
}

void delete_it(AVLTree avltree, int &index){
    cout<<"Enter the input"<<endl;
}

void demo(){
    AVLTree avlTree;
    avlTree.insert(10,"Vishal",1,2,"7876223214");
    avlTree.insert(20,"Sumit",1,2,"7018845818");
    avlTree.insert(30,"Kiran",1,2,"8628897201");
    avlTree.insert(15,"Dinesh",1,2,"9816314578");
    avlTree.insert(5,"Anjali",1,2,"7807586400");
    cout<<"Print data in sorted manner"<<endl;
    cout<<endl;
    cout<<"Now writing data to sst."<<endl;
    flush_data(avlTree);
}

void run(){
    int index = 0;
    AVLTree avlTree;
    int q;
    cout<<"Enter the number of queries you want to run"<<endl;
    cin>>q;
    string query;
    while(q--){
        cout<<"Enter the query. (For help enter -h)"<<endl;
        cin>>query;
        if(query=="-h"){
            help();
        }
        else if(query=="add"){
            add(avlTree, index);
        }
        else if(query=="delete"){
            delete_it(avlTree, index);
        }
        else{
            cout<<"Error in query!"<<endl;
        }
    }
}

int main(){
    
    // run();

    demo();

    return 0;
}