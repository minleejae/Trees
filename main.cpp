#include <iostream>
#include <stack>

using namespace std;

struct Node {
    int key;
    int height;
    Node *left;
    Node *right;

    Node() : key(0), height(1), left(nullptr), right(nullptr) {}
};

Node *getNode() {
    return new Node();
}


void insertBST(Node **T, int newKey) {
    //삽입 위치 찾기
    Node *p = *T, *q = nullptr;
    stack<Node *> stack;

    while (p != nullptr) {
        if (newKey == p->key) {
            cout << "i " << newKey << " : The key already exists" << '\n';
            return;
        }
        q = p;
        stack.push(q);
        if (newKey < p->key) p = p->left;
        else if (newKey > p->key) p = p->right;
    }

    //새로운 노드 만들어서 삽입
    Node *newNode = getNode();
    newNode->key = newKey;

    if (*T == nullptr) {
        *T = newNode;
    } else if (newKey < q->key) {
        q->left = newNode;
    } else {
        q->right = newNode;
    }


    while (!stack.empty()) {
        q = stack.top();
        stack.pop();

        if (q->left != nullptr && q->right != nullptr) {
            q->height = 1 + max(q->left->height, q->right->height);
        } else if (q->left != nullptr) {
            q->height = 1 + q->left->height;
        } else if (q->right != nullptr) {
            q->height = 1 + q->right->height;
        } else {
            q->height = 0;
        }
    }
}

int height(Node *T) {
    if (T == nullptr) return 0;
    return T->height;
}

int noNodes(Node *T) {
    if (T == nullptr) return 0;
    int cnt = 1;
    cnt += noNodes(T->left);
    cnt += noNodes(T->right);
    return cnt;
}

Node *maxNode(Node *T) {
    Node *p = T;
    while (p->right) {
        p = p->right;
    }
    return p;
}

Node *minNode(Node *T) {
    Node *p = T;
    while (p->left) {
        p = p->left;
    }
    return p;
}


void deleteBST(Node **T, int deleteKey) {
    //삭제할 노드 p,  삭제할 노드의 부모 노드 q
    Node *p = *T, *q = nullptr;
    stack<Node *> stack;

    while (p != nullptr) {
        if (deleteKey == p->key) break;
        q = p;
        stack.push(q);

        if (deleteKey < p->key) p = p->left;
        else p = p->right;
    }

    //삭제할 원소가 없는 경우
    if (p == nullptr) {
        cout << "d " << deleteKey << " : The key does not exist" << '\n';
        return;
    }

    // 삭제할 노드의 차수가 0인 경우(리프 노드)
    if (p->left == nullptr && p->right == nullptr) {
        //루트 노드만 있는 경우, 루트를 삭제
        if (q == nullptr) {
            delete *T;
            *T = nullptr;
        } else if (q->left == p) {
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
                Node *tmp = minNode(p->right);
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
        string flag;
        Node *r = nullptr;

        stack.push(p);

        int leftHeight = height(p->left);
        int rightHeight = height(p->right);

        if (leftHeight > rightHeight) {
            r = maxNode(p->left);
            flag = "LEFT";
        } else if (leftHeight < rightHeight) {
            r = minNode(p->right);
            flag = "RIGHT";
        } else {
            int leftNodes = noNodes(p->left);
            int rightNodes = noNodes(p->right);

            if (leftNodes >= rightNodes) {
                r = maxNode(p->left);
                flag = "LEFT";
            } else {
                r = minNode(p->right);
                flag = "RIGHT";
            }
        }

        p->key = r->key;
        if (flag == "LEFT") {
            deleteBST(&p->left, r->key);
        } else {
            deleteBST(&p->right, r->key);
        }
    }

    //height 구하기
    while (!stack.empty()) {
        q = stack.top();
        stack.pop();
        if (q->left != nullptr && q->right != nullptr) {
            q->height = 1 + max(q->left->height, q->right->height);
        } else if (q->left != nullptr) {
            q->height = 1 + q->left->height;
        } else if (q->right != nullptr) {
            q->height = 1 + q->right->height;
        } else {
            q->height = 0;
        }
    }
}


void inOrder(Node *T) {
    if (T == nullptr) return;
    inOrder(T->left);
    cout << T->key << ' ';
    inOrder(T->right);
}


int main() {
    Node *T = nullptr;

    //파일 입력
    freopen("BST-input.txt", "rt", stdin);
//    freopen("output_test2.txt", "w", stdout);

    char c;
    int num;
    while (cin >> c >> num) {
        if (c == 'i') {
            insertBST(&T, num);
        } else if (c == 'd') {
            deleteBST(&T, num);
        }
        inOrder(T);
        cout << '\n';
    }
    return 0;
}
