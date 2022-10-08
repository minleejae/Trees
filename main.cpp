#include <iostream>

using namespace std;

struct Node {
    int key;
    int height;
    Node *left;
    Node *right;
    Node() : key(0), height(0), left(nullptr), right(nullptr) {}
};


Node *getNode() {
    return new Node();
}


void insertBST(Node **Tree, int newKey) {
    //삽입 위치 찾기
    Node *p = *Tree, *q = nullptr;

    while (p != nullptr) {
        q = p;
        if (newKey < p->key) p = p->left;
        else if (newKey > p->key) p = p->right;
        else {
            cout << "i " << newKey << " : The key already exist" << '\n';
            return;
        }
    }

    //새로운 노드 만들어서 삽입
    Node *newNode = getNode();
    newNode->key = newKey;
    newNode->right = nullptr;
    newNode->left = nullptr;

    if (*Tree == nullptr) {
        *Tree = newNode;
    } else if (newKey < q->key) {
        q->left = newNode;
    } else {
        q->right = newNode;
    }
}

int height(Node *Tree) {
    if(Tree == nullptr) return 0;
    int leftHeight = height(Tree->left);
    int rightHeight = height(Tree->right);
    return 1 + max(leftHeight, rightHeight);
}

int noNodes(Node *Tree) {
    if (Tree == nullptr) return 0;
    int cnt = 1;
    cnt += noNodes(Tree->left);
    cnt += noNodes(Tree->right);
    return cnt;
}

Node *maxNode(Node *Tree) {
    Node *p = Tree;
    while (p->right) {
        p = p->right;
    }
    return p;
}

Node *minNode(Node *Tree) {
    Node *p = Tree;
    while (p->left) {
        p = p->left;
    }
    return p;
}


void deleteBST(Node **Tree, int deleteKey) {
    //삭제할 노드 p,  삭제할 노드의 부모 노드 q
    Node *p = *Tree, *q = nullptr;

    while (p != nullptr) {
        if (deleteKey == p->key) break;
        q = p;

        if (deleteKey < p->key) p = p->left;
        else p = p->right;
    }

    //삭제할 원소가 없음
    if (p == nullptr) {
        cout << "d " << deleteKey << " : The key does not exist" << '\n';
        return;
    }
    //루트 노드만 있는 경우, 루트를 삭제
    if (q == nullptr) {
        if (p->left == nullptr && p->left == nullptr) {
            delete *Tree;
            *Tree = nullptr;
            return;
        }
    }

    // 삭제할 노드의 차수가 0인 경우(리프 노드)
    if (p->left == nullptr && p->right == nullptr) {
        if (q->left == p) {
            delete q->left;
            q->left = nullptr;
        } else {
            delete q->right;
            q->right = nullptr;
        }
    }// 삭제할 노드의 차수가 1인 경우
    else if (p->left == nullptr || p->right == nullptr) {
        //삭제 해야 할 노드가 루트 노드고 차수가 1개인 경우
        if (q == nullptr) {
            if (p->left != nullptr) {
                Node *tmp = maxNode(p->left);
                p->key = tmp->key;
                deleteBST(&p->left, p->key);
            } else {
                Node *tmp = maxNode(p->right);
                p->key = tmp->key;
                deleteBST(&p->right, p->key);
            }
        }// 삭제 해야 할 노드가 루트 노드가 아니고, 차수가 1개인 노드인 경우
        else if (p->left != nullptr) {
            if (q->left == p) {
                q->left = p->left;

                delete p;
                p = nullptr;
            } else {
                q->right = p->left;

                delete p;
                p = nullptr;
            }
        }// p의 오른쪽 자식만 있는 경우
        else {
            if (q->left == p) {
                q->left = p->right;

                delete p;
                p = nullptr;
            } else {
                q->right = p->right;

                delete p;
                p = nullptr;
            }
        }


    } else { // 삭제할 노드의 차수가 2인 경우
        Node *r = maxNode(p->left);
        p->key = r->key;
        deleteBST(&p->left, r->key);
    }
}


void inOrder(Node *Tree) {
    if (Tree == nullptr) return;
    inOrder(Tree->left);
    cout << Tree->key << '\n';
    inOrder(Tree->right);
}


int main() {
    Node *Tree = nullptr;


    insertBST(&Tree, 50);
    insertBST(&Tree, 20);
    insertBST(&Tree, 30);
    insertBST(&Tree, 10);

    inOrder(Tree);


    cout << "Height : " << height(Tree->left) << '\n';
    cout << "-------\n";




    deleteBST(&Tree, 50);
    inOrder(Tree);
    cout << "Height : " << height(Tree->left) << '\n';
    cout << "-------\n";


    deleteBST(&Tree, 20);
    inOrder(Tree);
    cout << "Height : " << height(Tree->left) << '\n';
    cout << "-------\n";


    deleteBST(&Tree, 30);
    inOrder(Tree);
    cout << "Height : " << height(Tree->left) << '\n';
    cout << "-------\n";


    deleteBST(&Tree, 10);
    inOrder(Tree);
    cout << "Height : " << height(Tree) << '\n';
    cout << "-------\n";

    return 0;
}
