#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <string>
using namespace std;

struct Node {
public:
    int key;
    unordered_map<string, string> schema;
    Node* left;
    Node* right;
    int height;

    Node(int k, string field, string value) : key(k), left(nullptr), right(nullptr), height(1) {
        schema[field] = value;
    }
};

struct AVLTree {
    Node* root;

    AVLTree() : root(nullptr) {}

    int getHeight(Node* node) {
        if (!node) return 0;
        return node->height;
    }

    int getBalance(Node* node) {
        if (!node) return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = 1 + std::max(getHeight(y->left), getHeight(y->right));
        x->height = 1 + std::max(getHeight(x->left), getHeight(x->right));

        return x;
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = 1 + std::max(getHeight(x->left), getHeight(x->right));
        y->height = 1 + std::max(getHeight(y->left), getHeight(y->right));

        return y;
    }

    Node* insert(Node* node, int key, string field, string value) {
        if (!node) return new Node(key, field, value);

        if (key < node->key)
            node->left = insert(node->left, key, field, value);
        else if (key > node->key)
            node->right = insert(node->right, key, field, value);
        else
            return node;

        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

        int balance = getBalance(node);

        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    void insert(int key, string field, string value) {
        root = insert(root, key, field, value);
    }

    void inorderTraversal(Node* node) {
        if (node) {
            inorderTraversal(node->left);
            std::cout << node->key << " ";
            inorderTraversal(node->right);
        }
    }

    void inorderTraversal() {
        inorderTraversal(root);
    }
};

int main() {
    AVLTree avlTree;

    // avlTree.insert(10);
    // avlTree.insert(20);
    // avlTree.insert(30);
    vector<pair<string,string>> pairs;
    int n = 4;
    for(int i=0; i<n; i++){
        string field, value;
        cout<<"Enter the field value: ";
        cin>>field;
        cout<<"Enter the value for this field: ";
        cin>>value;
        avlTree.insert(i, field, value);
    }

    cout << "Inorder traversal of the constructed AVL tree is: ";
    avlTree.inorderTraversal();
    
    return 0;
}
