#include <iostream>
#include <stack>

using namespace std;

/*
 2분반 20191638 이민재 AVL 과제 코드 입니다.
 삽입 : 성공
 삭제 : 성공

 실행환경 : c++14 버전
 IDE : CLion으로 실행했습니다.
*/

// AVL의 각 노드를 표현하는 클래스
struct Node {
    int key;
    int height;
    int bf;
    Node *left;
    Node *right;

    Node() : key(0), height(1), bf(0), left(nullptr), right(nullptr) {}
};

// 새로운 노드를 생성하는 함수
Node *getNode() {
    return new Node();
}

//트리의 높이를 구하는 함수
int height(Node *T) {
    if (T == nullptr) return 0;
    return T->height;
}


// insertBST(T, newKey):  newKey의 삽입 위치를 검색하여 삽입 실행함.
bool insertBST(Node **T, int newKey);


// newKey부터 루트까지 BF를 다시 계산함
void checkBalance(Node **T, int newKey, string *rotationType, Node **p, Node **q) {
    stack<Node *> stack;
    Node *x = nullptr;
    Node *f = nullptr;

    *p = *T;

    //newKey를 발견할 때까지 BST 탐색하면서 stack에 추가
    while (*p != nullptr) {
        stack.push(*p);
        if (newKey == (*p)->key) break;
        if (newKey < (*p)->key) *p = (*p)->left;
        else *p = (*p)->right;
    }

    while (!stack.empty()) {
        *q = stack.top();
        stack.pop();
        (*q)->bf = height((*q)->left) - height((*q)->right);

        // 불균형으로 판명된 최초의 노드 발견 하여 x에 대입, f는 x의 부모 노드
        if (1 < (*q)->bf || (*q)->bf < -1) {
            if (x == nullptr) {
                x = *q;
                f = !stack.empty() ? stack.top() : nullptr;
                break;
            }
        }
    }



    //불균형이 발생하지 않으면, rotationType은 “NO”로 하고, p와 q는 널값을 리턴
    if (x == nullptr) {
        *p = nullptr;
        *q = nullptr;
        *rotationType = "NO";
        return;
    }

    *p = x;
    *q = f;


    if (1 < x->bf) {
        if (x->left->bf < 0) {
            *rotationType = "LR";
        } else {
            *rotationType = "LL";
        }
    } else {
        if (x->right->bf > 0) {
            *rotationType = "RL";
        } else {
            *rotationType = "RR";
        }
    }
}


//  rotationType에 따라, p를 루트로 하는 서브트리를 회전하고 BF의 변경이 필요한 노드의 BF를 다시 계산함
void rotateTree(Node **T, string rotationType, Node *p, Node **q) {
    Node *a = nullptr;
    Node *b = nullptr;
    Node *c = nullptr;

    // 루트노드부터 p까지 높이와 bf를 다시 업데이트 하기 위한 스택
    stack<Node *> stack;
    Node *ptr = *T;
    while (ptr != p) {
        if (ptr->key == p->key) break;
        stack.push(ptr);

        if (p->key < ptr->key) ptr = ptr->left;
        else ptr = ptr->right;
    }


    if (rotationType == "LL") {
        //재균형
        a = p;
        b = p->left;
        a->left = b->right;
        b->right = a;

        //p의 부모노드 q가 nullptr인 경우 트리의 루트노드가 b가 되도록 함, 아니면 q와 p의 키값을 비교하고 서브트리 지정
        if (*q == nullptr) *T = b;
        else if ((*q)->key > p->key) (*q)->left = b;
        else (*q)->right = b;


        //노드 높이, 밸런싱팩터 재계산
        a->height = 1 + max(height(a->left), height(a->right));
        a->bf = height(a->left) - height(a->right);
        b->height = 1 + max(height(b->left), height(b->right));
        b->bf = height(b->left) - height(b->right);

    } else if (rotationType == "LR") {
        //재균형
        a = p;
        b = p->left;
        c = b->right;

        b->right = c->left;
        a->left = c->right;
        c->left = b;
        c->right = a;

        //p의 부모노드 q가 nullptr인 경우 트리의 루트노드가 c가 되도록 함, 아니면 q와 p의 키값을 비교하고 서브트리 지정
        if (*q == nullptr) *T = c;
        else if ((*q)->key > p->key) (*q)->left = c;
        else (*q)->right = c;

        //노드 높이, 밸런싱팩터 재계산
        a->height = 1 + max(height(a->left), height(a->right));
        a->bf = height(a->left) - height(a->right);
        b->height = 1 + max(height(b->left), height(b->right));
        b->bf = height(b->left) - height(b->right);
        c->height = 1 + max(height(c->left), height(c->right));
        c->bf = height(c->left) - height(c->right);

    } else if (rotationType == "RR") {
        //재균형
        a = p;
        b = p->right;
        a->right = b->left;
        b->left = a;

        //p의 부모노드 q가 nullptr인 경우 트리의 루트노드가 b가 되도록 함, 아니면 q와 p의 키값을 비교하고 서브트리 지정
        if (*q == nullptr) *T = b;
        else if ((*q)->key > p->key) (*q)->left = b;
        else (*q)->right = b;

        //노드 높이, 밸런싱팩터 재계산
        a->height = 1 + max(height(a->left), height(a->right));
        a->bf = height(a->left) - height(a->right);
        b->height = 1 + max(height(b->left), height(b->right));
        b->bf = height(b->left) - height(b->right);

    } else if (rotationType == "RL") {
        //재균형
        a = p;
        b = p->right;
        c = b->left;

        b->left = c->right;
        a->right = c->left;
        c->right = b;
        c->left = a;

        //p의 부모노드 q가 nullptr인 경우 트리의 루트노드가 c가 되도록 함
        if (*q == nullptr) *T = c;
        else if ((*q)->key > p->key) (*q)->left = c;
        else (*q)->right = c;

        //노드 높이, 밸런싱팩터 재계산
        a->height = 1 + max(height(a->left), height(a->right));
        a->bf = height(a->left) - height(a->right);
        b->height = 1 + max(height(b->left), height(b->right));
        b->bf = height(b->left) - height(b->right);
        c->height = 1 + max(height(c->left), height(c->right));
        c->bf = height(c->left) - height(c->right);
    }


    // 재균형 이후 노드의 height, balancing factor 재계산
    while (!stack.empty()) {
        ptr = stack.top();
        stack.pop();
        ptr->height = 1 + max(height(ptr->left), height(ptr->right));
        ptr->bf = height(ptr->left) - height(ptr->right);
    }
}


// AVL 삽입 알고리즘
bool insertAVL(Node **T, int newKey) {
    if (T == nullptr) { //트리의 원소가 존재하지 않는 경우 원소 추가 후 바로 리턴
        insertBST(T, newKey);
        return true;
    }

    string rotationType = "NO";
    Node *p = nullptr;
    Node *q = nullptr;

    //Step1: BST 삽입 알고리즘 실행
    //이미 원소가 존재하는 경우 false return
    bool found = insertBST(T, newKey);
    if (found) return false;


    //Step 2 :균형 검사
    checkBalance(T, newKey, &rotationType, &p, &q);
    //rotationType 출력
    cout << rotationType << ' ';


    //Step 3: rebalancing 실행
    if (rotationType != "NO") {
        rotateTree(T, rotationType, p, &q);
    }
}


// 자신을 포함한 하위 노드의 개수를 구하는 함수
int noNodes(Node *T) {
    if (T == nullptr) return 0;
    int cnt = 1;
    cnt += noNodes(T->left);
    cnt += noNodes(T->right);
    return cnt;
}

// deleteKey의 위치를 검색하여 삭제 실행함
Node *deleteBST(Node **T, int deleteKey);


// AVL 삭제 알고리즘
void deleteAVL(Node **T, int deleteKey) {
    string rotationType = "NO";
    Node *parent = nullptr;
    Node *p = nullptr;
    Node *q = nullptr;


    // deleteKey에 해당하는 값이 루트노드인 경우
    if(*T != nullptr && (*T)->key == deleteKey){
        parent = deleteBST(T, deleteKey);

        // 삭제되는 노드의 부모가 nullptr이 아닌 경우
        if(parent != nullptr){
            // Step2: 균형 검사
            checkBalance(T, parent->key, &rotationType, &p, &q);
        }
        cout << rotationType << ' ';

        //Step 3: rebalancing 실행
        if (rotationType != "NO") {
            rotateTree(T, rotationType, p, &q);
        }
        return;
    }

    // deleteKey값에 해당하는 노드가 존재하지 않는 경우
    parent = deleteBST(T, deleteKey);
    if (parent == nullptr) {
        return;
    }

    // Step2: 균형 검사
    checkBalance(T, parent->key, &rotationType, &p, &q);

    //rotationType 출력
    cout << rotationType << ' ';

    //Step 3: rebalancing 실행
    if (rotationType != "NO") {
        rotateTree(T, rotationType, p, &q);
    }
    return;
}


// inorder로 순회하며 출력하는 함수
void inOrderBST(Node *T) {
    if (T == nullptr) return;
    inOrderBST(T->left);
    cout << "(" << T->key << ", " << T->bf << ") ";
    inOrderBST(T->right);
}


int main() {
    Node *T = nullptr;

    //파일 입력
    freopen("AVL-input.txt", "rt", stdin);
    freopen("output_test2.txt", "w", stdout);

    char c;
    int num;
    while (cin >> c >> num) {
        if (c == 'i') {
            insertAVL(&T, num);
        } else if (c == 'd') {
            deleteAVL(&T, num);
        }
        inOrderBST(T);
        cout << '\n';
    }
    return 0;
}


// insertBST(T, newKey):  newKey의 삽입 위치를 검색하여 삽입 실행함. 이미 있는 원소를 추가하는 경우 true리턴
bool insertBST(Node **T, int newKey) {
    //삽입 위치 찾기
    Node *p = *T, *q = nullptr;
    stack<Node *> stack;

    while (p != nullptr) {
        if (newKey == p->key) {
            cout << "i " << newKey << " : The key already exists" << '\n';
            return true;
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

        q->height = 1 + max(height(q->left), height(q->right));
    }

    return false;
}


// deleteKey의 위치를 검색하여 삭제하여 부모노드 반환
Node *deleteBST(Node **T, int deleteKey) {
    //삭제할 노드 p,  삭제할 노드의 부모 노드 q
    Node *p = *T, *q = nullptr, *temp = nullptr;
    stack<Node *> stack;

    while (p != nullptr && deleteKey != p->key) {
        q = p;
        stack.push(q);

        if (deleteKey < p->key) p = p->left;
        else p = p->right;
    }

    //삭제할 원소가 없는 경우
    if (p == nullptr) {
        cout << "d " << deleteKey << " : The key does not exist" << '\n';
        return nullptr;
    }

    // 삭제할 노드의 차수가 2인 경우
    if (p->left != nullptr && p->right != nullptr) {
        stack.push(p);
        temp = p;

        if (height(p->left) <= height(p->right)) {
            p = p->right;
            while (p->left != nullptr) {
                stack.push(p);
                p = p->left;
            }
        } else {
            p = p->left;
            while (p->right != nullptr) {
                stack.push(p);
                p = p->right;
            }
        }

        temp->key = p->key;
        q = stack.top();
    }

    // 삭제되는 노드의 부모 노드q
    Node *ret = q;

    // p의 차수가 0인 경우
    if (p->left == nullptr && p->right == nullptr) {
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
    } else { // 차수가 1인 경우
        if (p->left != nullptr) {
            if (q == nullptr) {
                *T = (*T)->left;
            } else if (q->left == p) {
                q->left = p->left;
            } else {
                q->right = p->left;
            }
        } else {
            if (q == nullptr) {
                *T = (*T)->right;
            } else if (q->left == p) {
                q->left = p->right;
            } else {
                q->right = p->right;
            }
        }
    }

    //height 구하기
    while (!stack.empty()) {
        q = stack.top();
        stack.pop();
        q->height = 1 + max(height(q->left), height(q->right));
    }

    return ret;
}
