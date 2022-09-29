#include <iostream>

using namespace std;

struct Node{
    int data;
    Node* left;
    Node* right;
    Node(int _data, Node* _left, Node* _right): data(_data), left(_left), right(_right){}
};

void insert(Node** root, int value){
    //삽입 위치 찾기
    Node *p = *root, *q = nullptr;

    while(p!= nullptr){
        q = p;
        if(value < p->data) p = p->left;
        else if(value > p->data) p = p->right;
        else return;
    }

    //새로운 노드 만들어서 삽입
    Node *newNode = new Node(value, nullptr, nullptr);

    if(q == nullptr){
        *root = newNode;
    }
    else if(value < q->data){
        q->left = newNode;
    }
    else{
        q->right = newNode;
    }
}


void inOrder(Node* root){
    if(root == nullptr) return;
    inOrder(root->left);
    cout << root->data<<'\n';
    inOrder(root->right);
}


int main() {
    Node* root = nullptr;


    insert(&root, 50);
    insert(&root, 20);
    insert(&root, 30);
    insert(&root, 10);
    insert(&root, 30);
    insert(&root, 30);


    inOrder(root);
    return 0;
}
