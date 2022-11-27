#include <iostream>
#include <stack>

#define M_MAX 6

using namespace std;

//B 트리 노드를 표현하는 클래스 또는 구조체
struct Node {
    int n;
    int K[M_MAX];
    Node *P[M_MAX];
};

Node *getNode() {
    return new Node();
}


// key에 대한 검색 수행, key 발견 여부와 방문한 정점 경로를 담은 stack리턴, 주어진 stack이 null이 아닌 스택이 주어지면 해당 스택에 경로를 저장
bool searchPath(Node *T, int m, int key, stack<Node *> **stack) {
    if (T == nullptr) {
        return false;
    }


    if (*stack == nullptr) {
        *stack = new ::stack<Node *>();
    }

    Node *x = T;
    int i;
    do {
        i = 1;
        while (i <= x->n && key > x->K[i]) {
            i++;
        }

        (*stack)->push(x);
        //삽입할 키를 발견함, 삽입 불가
        if (i <= x->n && key == x->K[i]) {
            return true;
        }

    } while ((x = x->P[i - 1]) != nullptr);
    return false;
}

// newKey를 노드 x의 적당한 위치에 삽입, y는 x에서 newKey위치 다음 포인터가 가리킬 노드(자식 노드에서 split되어 새로 만들어진 노드)
void insertKey(Node **T, int m, Node *x, Node *y, int &newKey) {
    //newKey보다 큰 키들을, 오른쪽으로 한 칸씩 이동
    int i = x->n;
    while (i >= 1 && newKey < x->K[i]) {
        x->K[i + 1] = x->K[i];
        x->P[i + 1] = x->P[i];
        i--;
    }

    if (x->K[i] < newKey) {
        //newKey를 삽입
        x->K[i + 1] = newKey;
        x->P[i + 1] = y;
        x->n++;
    } else {
        x->K[i] = newKey;
        x->P[i - 1] = y;
        x->n++;
    }
}

// newKey를 x에 삽입 후 split을 수행, y는 x에서 newKey위치 다음 포인터가 가리킬 노드(자식 노드에서 split되어 새로 만들어진 노드), split 후 부모 노드에 삽입되어야 할 키값과 분리된 새로운 노드를 반환
void splitNode(Node **T, int m, Node **x, Node **y, int &newKey) {
    Node *tempNode = getNode();
//    copy x
    tempNode->n = (*x)->n;
    for (int i = 0; i <= (*x)->n; i++) {
        tempNode->P[i] = (*x)->P[i];
        tempNode->K[i] = (*x)->K[i];
    }

    insertKey(T, m, tempNode, *y, newKey);

    //분할 기준인 중앙값
    int centerKey = tempNode->K[tempNode->n / 2 + 1];
    newKey = centerKey;

    //centerKey이전 값을 노드 x로 복사
    (*x)->n = 0;
    int i = 1;
    while (tempNode->K[i] < centerKey) {
        (*x)->K[i] = tempNode->K[i];
        (*x)->P[i - 1] = tempNode->P[i - 1];
        i++;
        (*x)->n++;
    }
    (*x)->P[i - 1] = tempNode->P[i - 1];

    //centerKey이후 값을 노드 newNode로 복사
    Node *newNode = getNode();
    i++;
    while (i <= tempNode->n) {
        newNode->n++;
        newNode->K[newNode->n] = tempNode->K[i];
        newNode->P[newNode->n - 1] = tempNode->P[i - 1];
        i++;
    }
    newNode->P[newNode->n] = tempNode->P[i - 1];
    *y = newNode;
}

// B-tree의 삽입 알고리즘
void insertBT(Node **T, int m, int newKey) {
    // 루트 노드를 생성
    if (*T == nullptr) {
        *T = getNode();
        (*T)->K[1] = newKey;
        (*T)->n = 1;
        return;
    }

    // newKey를 삽입할 노드의 경로를 탐색하며, 스택에 경로 저장
    bool found;
    stack<Node *> *stack = nullptr;
    found = searchPath(*T, m, newKey, &stack);

    //newKey를 발견함, 삽입 불가
    if (found) {
        cout << "i " << newKey << " : The key already exists" << '\n';
        return;
    }

    // newKey가 없으므로, T에 삽입 (이제 x는 null)
    bool finished = false;
    Node *x = stack->top();
    stack->pop();

    Node *y = nullptr; //새로 분할된 노드를 담을 변수

    do {//Overflow 발생 여부 검사
        if (x->n < m - 1) { //Overflow 발생 안함, newKey를 노드 x의 올바른 위치에 삽입
            insertKey(T, m, x, y, newKey);
            finished = true;
        } else { //Overflow 발생
            //x를 newKey 기준으로 분할, 분할된 노드 반환
            splitNode(T, m, &x, &y, newKey);

            if (!(stack->empty())) {
                x = stack->top();
                stack->pop();
            } else {
                *T = getNode();
                (*T)->n++;
                (*T)->K[1] = newKey;
                (*T)->P[0] = x;
                (*T)->P[1] = y;
                finished = true;
            }
        }
    } while (!finished);
}


// oldKey를 x에서 제거 수행
void deleteKey(Node **T, int m, Node **x, int oldKey) {
    //oldKey의 위치 i를 탐색
    int i = 1;
    while (oldKey > (*x)->K[i]) {
        i++;
    }


    while (i <= (*x)->n) {
        (*x)->K[i] = (*x)->K[i + 1];
        (*x)->P[i] = (*x)->P[i + 1];
        i++;
    }
    (*x)->n--;
}

// 노드 x와 해당 노드의 부모 노드 y가 주어졌을 때, x의 best 형제 노드를 반환
int bestSibling(Node **T, int m, Node *x, Node *y) {
    //y에서 x의 위치 i를 탐색
    int i = 0;
    while (y->P[i] != x) {
        i++;
    }

    //바로 인접한 두 형제 중, 키의 개수가 많은 형제를 bestSibling으로 선택함.
    if (i == 0) {
        return i + 1;
    } else if (i == y->n || y->P[i]->n >= y->P[i + 1]->n)
        return i - 1;
    return i + 1;
}

// 노드 x와 해당 노드의 부모 노드 y 그리고 y에서의 best sibling의 인덱스
// bestSibling이 주어졌을 때 x와 best sibling 노드간의 키 재분배 수행
void redistributeKeys(Node **T, int m, Node **x, Node **y, int bestSib) {
    int i = 0;
    while ((*y)->P[i] != *x) {
        i++;
    }

    Node *bestNode = (*y)->P[bestSib];

    //키 재분배를 하며 포인터도 변경
    if (bestSib < i) {
        int lastKey = bestNode->K[bestNode->n];
        insertKey(T, m, *x, nullptr, (*y)->K[i]);
        (*x)->P[0] = bestNode->P[bestNode->n];
        bestNode->P[bestNode->n] = nullptr;
        deleteKey(T, m, &bestNode, lastKey);
        (*y)->K[i] = lastKey;
    } else {
        int firstKey = bestNode->K[1];
        insertKey(T, m, *x, nullptr, (*y)->K[i + 1]);
        (*x)->P[m / 2] = bestNode->P[0];
        bestNode->P[0] = bestNode->P[1];
        deleteKey(T, m, &bestNode, firstKey);
        (*y)->K[i + 1] = firstKey;
    }
}

// 노드 x와 해당 노드의 부모 노드 y, 그리고 y에서의 best sibling의 인덱스
// bestSibling이 주어졌을 때 x와 best sibling 노드간의 합병 수행
void mergeNode(Node **T, int m, Node **x, Node **y, int bestSib) {
    int i = 0;
    while ((*y)->P[i] != *x) {
        i++;
    }

    Node *bestNode = (*y)->P[bestSib];

    //왼쪽 형제 노드로의 병합만 고려할 수 있도록 swap
    if (bestSib > i) {
        int tmp = i;
        i = bestSib;
        bestSib = tmp;

        Node *tmpNode = *x;
        *x = bestNode;
        bestNode = tmpNode;
    }

    //왼쪽 형제노드와 병합
    bestNode->K[bestNode->n + 1] = (*y)->K[i];
    bestNode->n++;
    int j = 1;

    while (j <= (*x)->n) {
        bestNode->K[bestNode->n + 1] = (*x)->K[j];
        bestNode->P[bestNode->n] = (*x)->P[j - 1];
        bestNode->n++;
        j++;
    }
    bestNode->P[bestNode->n] = (*x)->P[(*x)->n];
    deleteKey(T, m, y, (*y)->K[i]);

    delete *x;
}

bool isTerminalNode(int stackSize, Node *T) {
    Node *x = T;
    int height = 1;
    while (x->P[1] != nullptr) {
        height++;
        x = x->P[1];
    }

    return (stackSize == height);
}

//B-tree의 삭제 알고리즘
void deleteBT(Node **T, int m, int oldKey) {
    // oldKey가 있던 노드의 경로를 탐색하며 스택에 경로 저장
    bool found;
    stack<Node *> *stack = nullptr;

    found = searchPath(*T, m, oldKey, &stack);

    //oldKey를 발견 못함, 삭제 불가
    if (!found) {
        cout << "d " << oldKey << " : The key does not exist" << '\n';
        return;
    }

    int stackSize = stack->size();
    Node *x = stack->top();
    stack->pop();


    //oldKey를 내부 노드에서 발견
    if (!isTerminalNode(stackSize, *T)) {
        Node *internalNode = x;

        int i = 0;
        for (int j = 1; j <= internalNode->n; j++) {
            if (oldKey == internalNode->K[j]) {
                i = j;
                break;
            }
        }
        stack->push(x);
        searchPath(x->P[i], m, x->K[i], &stack);

        x = stack->top();
        stack->pop();

        int temp = internalNode->K[i];
        internalNode->K[i] = x->K[1];
        x->K[1] = temp;

        deleteKey(T, m, &x, x->K[1]);
    } else {
        //노드 x에서 oldKey를 삭제
        deleteKey(T, m, &x, oldKey);
    }

    bool finished = false;

    Node *y = nullptr;
    if (!stack->empty()) {
        y = stack->top();
        stack->pop();
    }

    do {
        if (*T == x || x->n >= (m - 1) / 2) { //underflow 발생하지 않음
            finished = true;
        } else { // underflow 발생
            //키 재분배 또는 노드 합병을 위한 형제 노드를 결정
            int bestSib = bestSibling(T, m, x, y);

            if (y->P[bestSib]->n > (m - 1) / 2) { // bestSibling에서 재분배
                redistributeKeys(T, m, &x, &y, bestSib);
                finished = true;
            } else { // bestSibling과 노드 합병
                mergeNode(T, m, &x, &y, bestSib);

                x = y;

                if (!stack->empty()) {
                    y = stack->top();
                    stack->pop();
                } else {
                    finished = true;
                }
            }
        }
    } while (!finished);

    if (y != nullptr && y->n == 0) { //y에 키가 없음(비어있음)
        (*T) = y->P[0];
        delete y; // old root 삭제, 트리 높이가 하나 줄어듦
    }
}

// 출력을 위한 B-tree의 inorder 순회 알고리즘
void inorderBT(Node *T, int m) {
    if (T == nullptr || T->n == 0) return;
    for (int i = 0; i <= T->n; i++) {
        if (T->P[i] != nullptr)
            inorderBT(T->P[i], m);
        if (i != T->n)
            cout << T->K[i + 1] << ' ';
    }
}



int main() {
    Node *T = nullptr;
    char c;
    int num;
    //파일 입력
    FILE *fp = freopen("BT-input.txt", "rt", stdin);
    while (cin >> c >> num) {
        if (c == 'i') {
            insertBT(&T, 3, num);
        } else if (c == 'd') {
            deleteBT(&T, 3, num);
        }
        inorderBT(T, 3);
        cout << '\n';
    }
    fclose(fp);


//    // m이 4일 때 결과
    T = nullptr;
    fseek(fp, 0, SEEK_SET);
    while (cin >> c >> num) {
        if (c == 'i') {
            insertBT(&T, 4, num);
        } else if (c == 'd') {
            deleteBT(&T, 4, num);
        }
        inorderBT(T, 4);
        cout << '\n';
    }
    fclose(fp);

    return 0;
}
