
// 12150981 박중규 
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// node 클래스
class node {
public:
    // 부모, 자식 node 포인터
    node* p = nullptr;
    node* l = nullptr;
    node* r = nullptr;

    // 컬러 데이터 : 'b' == 검정, 'r' == 빨강
    char* color;

    // 환자정보 데이터
    int* id;
    string* name;
    string* num;
    int* add_x;
    int* add_y;
    vector<string> disease;
    int* pay;

    // 생성자
    node(int id, string name, string num, int add_x, int add_y, string disease, int pay) {
        this->color = new char;
        *this->color = 'r';

        this->id = new int;
       *this->id = id;
       this->name = new string;
        *this->name = name;
        this->num = new string;
        *this->num = num;
        this->add_x = new int;
        *this->add_x = add_x;
        this->add_y = new int;
        *this->add_y = add_y;
        this->disease.push_back(disease);
        this->pay = new int;
        *this->pay = pay;
    }
};

// Red-Black Tree 클래스
class RBT {
public:
    vector<node*> nodelist;
    node* root = nullptr;

    // 신규 가입을 위한 메서드. 'checkDoublered', 'restructuring', 'recoloring'은 삽입 시 RBT의 프로퍼티를 만족시키기 위한 메서드
    void insert(node* n);
    void insertForSearch(node* new_n, node* n);
    void checkDoublered(node* new_n);
    void restructuring(node* new_n);
    void recoloring(node* new_n);

    // 환자 검색을 위한 메서드
    void search(int id, node* n);
 
    // 추가 진료를 위한 메서드
    void addForSearch(int id, string DI, int C, node* n);

    // 유행병 조사를 위한 메서드
    void epiForSearch(string DI);

    // 노드의 깊이를 구하기 위한 메서드
    void printDepthAndInfo(int id, node* n);
    int returnDepth(int id, node* n);

};

RBT* rbt = new RBT();

// 신규 가입, 환자 검색, 추가 진료 메서드를 위한 변수
bool flag = false;
// 깊이 메서드를 위한 변수
int depth = 0;

int main() {
   
    int n;
    cin >> n;

    char cmd;
    while (n--) {
        cin >> cmd;
        
        // 신규 가입인 경우
        if (cmd == 'I') {
            int K;
            string N;
            string H;
            int Ax;
            int Ay;
            string DI;
            int C;

            cin >> K;
            cin >> N;
            cin >> H;
            cin >> Ax;
            cin >> Ay;
            cin >> DI;
            cin >> C;

            // 신규 환자 노드 생성
            node* n = new node(K, N, H, Ax, Ay, DI, C);
            // 삽입 메서드 실행
            rbt->insert(n);

            // flag == true : 신규 가입 성공
            if (flag) { 
                // 노드의 깊이와 1 출력
                cout << rbt->returnDepth(K, rbt->root);
                cout << " 1" << endl;

                // flag 초기화
                flag = false;
            }
            // flag == false : 신규 가입 실패(동일한 환자 번호 존재)
            else {
                // 노드의 깊이와 0 출력
                cout << rbt->returnDepth(K, rbt->root);
                cout << " 0" << endl;
            }
        }
        // 환자 검색인 경우
        else if (cmd == 'F') {
            int K;

            cin >> K;

            if (rbt->nodelist.size() == 0) {
                cout << "Not found" << endl;
            }
            else {
                // 탐색 메서드 실행
                rbt->search(K, rbt->root);

                // flag == true : 환자 검색 성공
                if (flag) {
                    // 해당 환자의 깊이와 정보를 출력
                    rbt->printDepthAndInfo(K, rbt->root);

                    // flag 초기화
                    flag = false;
                }
            }
        }
        // 추가 진료인 경우
        else if (cmd == 'A') {
            int K;
            string DI;
            int C;

            cin >> K;
            cin >> DI;
            cin >> C;

            if (rbt->nodelist.size() == 0) {
                cout << "Not found" << endl;
            }
            else {
                // 추가 진료 메서드 실행
                rbt->addForSearch(K, DI, C, rbt->root);

                // flag == true : 환자 검색 성공
                if (flag) {
                    // 해당 환자의 깊이 출력
                    cout << rbt->returnDepth(K, rbt->root) << endl;

                    // flag 초기화
                    flag = false;
                }
            }
        }
        // 유행병 조사인 경우
        else if (cmd == 'E') {
            string DI;

            cin >> DI;

            // 유행병 조사 메서드 실행
            rbt->epiForSearch(DI);
        }   
    }
}

// 삽입 메서드
void RBT::insert(node* n) {
    
    // 노드 개수가 0개인 경우 -> 해당 노드를 root 노드로 지정 후 삽입과정
    if (nodelist.size() == 0) {
        nodelist.push_back(n);
        this->root = n;
        *n->color = 'b';

        // leaf 노드를 다음과 같이 id값을 -1로 하여 구분함. leaf 노드의 컬러값은 black으로
        node* leaf_n1 = new node(-1, "", "", 0, 0, "", 0);
        node* leaf_n2 = new node(-1, "", "", 0, 0, "", 0);
        *leaf_n1->color = 'b';
        *leaf_n2->color = 'b';
        n->l = leaf_n1;
        n->r = leaf_n2;

        // 삽입 성공
        flag = true;
    }
    // 노드 개수가 1개이상
    else {
        // 신규 노드와 루트 노드부터 비교시작
        insertForSearch(n, this->root);
    }
}

// 노드 개수가 1개 이상일 때 실행되는 삽입 메서드
void RBT::insertForSearch(node* new_n, node* n) {

    // 환자 번호가 같은 노드가 있다면 그대로 종료(flag == false)
    if (*new_n->id == *n->id) {
    }
    // 신규 환자 번호 > 현재 노드 환자 번호
    else if (*new_n->id > *n->id) {
        // 현재 노드의 오른쪽 자식이 leaf노드 일 때
        if (*n->r->id == -1) {
            // 현재 노드의 오른쪽 자식으로 삽입
            new_n->p = n;
            n->r = new_n;
            nodelist.push_back(new_n);

            node* leaf_n1 = new node(-1, "", "", 0, 0, "", 0);
            node* leaf_n2 = new node(-1, "", "", 0, 0, "", 0);
            *leaf_n1->color = 'b';
            *leaf_n2->color = 'b';

            new_n->l = leaf_n1;
            new_n->r = leaf_n2;
            leaf_n1->p = new_n;
            leaf_n2->p = new_n;

            // 더블레드 체크 메서드 수행
            checkDoublered(new_n);

            // 삽입 성공
            flag = true;
        }
        // 현재 노드의 오른쪽 자식이 internal노드 일 때
        else {
            // 현재 노드를 오른쪽 자식으로 바꾸어주고, 삽입 메서드 재귀수행
            insertForSearch(new_n, n->r);
        }
    }
    // 신규 환자 번호 < 현재 노드 환자 번호
    else {
        // 현재 노드의 왼쪽 자식이 leaf노드 일 때
        if (*n->l->id == -1) {
            // 현재 노드의 왼쪽 자식으로 삽입
            new_n->p = n;
            n->l = new_n;
            nodelist.push_back(new_n);

            node* leaf_n1 = new node(-1, "", "", 0, 0, "", 0);
            node* leaf_n2 = new node(-1, "", "", 0, 0, "", 0);
            *leaf_n1->color = 'b';
            *leaf_n2->color = 'b';

            new_n->l = leaf_n1;
            new_n->r = leaf_n2;
            leaf_n1->p = new_n;
            leaf_n2->p = new_n;

            // 더블레드 체크 메서드 수행
            checkDoublered(new_n);

            // 삽입 성공
            flag = true;
        }
        // 현재 노드의 왼쪽 자식이 internal노드 일 때
        else {
            // 현재 노드를 왼쪽 자식으로 바꾸어주고, 삽입 메서드 재귀수행
            insertForSearch(new_n, n->l);
        }
    }
}

// 더블레드 체크 메서드
void RBT::checkDoublered(node* new_n) {
    
    // 부모가 검정이면 그대로 종료
    if (*new_n->p->color == 'b') {
    }
    // 부모가 빨강인 경우
    else {
        node* p = new_n->p;
        node* grand_p = p->p;

        // 부모노드가 할아버지노드의 왼쪽 자식인 경우
        if (p == grand_p->l) {
            // 엉클노드가 검정일 경우
            if (*grand_p->r->color == 'b') {
                // restructuring 메서드 수행 후 종료
                restructuring(new_n);
            }
            // 엉클노드가 빨강일 경우
            else {
                // recoloring 메서드 수행
                recoloring(new_n);

                // recoloring 이후, 할아버지 노드가 root면 색깔 바꿔주고 종료
                if (grand_p == this->root) {
                    *this->root->color = 'b';
                }
                // 아니면 할아버지 노드에 대해 더블체크 메서드 재귀수행
                else {
                    checkDoublered(grand_p);
                }
            }
        }
        // 부모노드가 할아버지노드의 오른쪽 자식인 경우
        else {
            // 엉클노드가 검정일 경우
            if (*grand_p->l->color == 'b') {
                // restructuring 메서드 수행 후 종료
                restructuring(new_n);
            }
            // 엉클노드가 빨강일 경우
            else {
                // recoloring 메서드 수행
                recoloring(new_n);

                // recoloring 이후, 할아버지 노드가 root면 색깔 바꿔주고 종료
                if (grand_p == this->root) {
                    *this->root->color = 'b';
                }
                // 아니면 할아버지 노드에 대해 더블체크 메서드 재귀수행
                else {
                    checkDoublered(grand_p);
                }
            }

        }
    }
}

// restructuring 메서드
void RBT::restructuring(node* new_n) {

    // 편의를 위한 변수 정의
    node* n = new_n;
    node* p = n->p;
    node* grand_p = p->p;

    node* x = nullptr;
    node* y = nullptr;
    node* z = nullptr;

    node* child_1 = nullptr;
    node* child_2 = nullptr;
    node* child_3 = nullptr;
    node* child_4 = nullptr;
    

    // 자식, 부모, 할아버지 노드의 번호에 따라 x, y, z, child_1, child_2, child_3, child_4 적절히 지정

    // 자식 노드 번호 > 부모 노드 번호
    if (*n->id > *p->id) {
        // 부모 노드 번호 > 할아버지 노드 번호
        if (*p->id > *grand_p->id) {
            x = grand_p;
            y = p;
            z = n;

            child_1 = x->l;
            child_2 = y->l;
            child_3 = z->l;
            child_4 = z->r;

        }
        // 부모 노드 번호 < 할아버지 노드 번호
        else {
            x = p;
            y = n;
            z = grand_p;

            child_1 = x->l;
            child_2 = y->l;
            child_3 = y->r;
            child_4 = z->r;

        }
    }
    // 자식 노드 번호 < 부모 노드 번호
    else {
        // 부모 노드 번호 < 할아버지 노드 번호
        if (*p->id < *grand_p->id) {
            x = n;
            y = p;
            z = grand_p;

            child_1 = x->l;
            child_2 = x->r;
            child_3 = y->r;
            child_4 = z->r;

        }
        // 부모 노드 번호 > 할아버지 노드 번호
        else {
            x = grand_p;
            y = n;
            z = p;

            child_1 = x->l;
            child_2 = y->l;
            child_3 = y->r;
            child_4 = z->r;

        }
    }

    // 각 경우에 수에 따라 x, y, z 지정 이후 부모-자식 관계 재정립
    y->l = x;
    y->r = z;

    if (grand_p == this->root) {
        this->root = y;
        y->p = nullptr;
    }
    else {
        y->p = grand_p->p;
        if (grand_p->p->l == grand_p) {
            grand_p->p->l = y;
        }
        else {
            grand_p->p->r = y;
        }
    }

    x->l = child_1;
    child_1->p = x;
    x->r = child_2;
    child_2->p = x;
    x->p = y;

    z->l = child_3;
    child_3->p = z;
    z->r = child_4;
    child_4->p = z;
    z->p = y;

    // restructuring에 따라 컬러 재정립
    *x->color = 'r';
    *z->color = 'r';
    *y->color = 'b';

}

// recoloring 메서드
void RBT::recoloring(node* new_n) {

    // recoloring에 따라 컬러 재정립
    node* n = new_n;
    node* p = n->p;
    node* grand_p = p->p;

    *p->color = 'b';
    *grand_p->color = 'r';
    if (p == grand_p->r) {
        *grand_p->l->color = 'b';
    }
    else {
        *grand_p->r->color = 'b';
    }
}

// 환자 검색 메서드
void RBT::search(int id, node* n) {
    // 환자 검색 성공
    if (id == *n->id) {
        flag = true;
    }
    // 찾는 환자 번호 > 현재 노드 환자 번호
    else if (id > * n->id) {
        // 현재 노드의 오른쪽 자식이 leaf노드이면 -> 환자 존재 X
        if (*n->r->id == -1) {
            cout << "Not found" << endl;
        }
        // 현재 노드의 오른쪽 자식이 internal노드이면
        else {
            // 현재 노드를 오른쪽 자식으로 바꾸어주고, 검색 메서드 재귀수행
            search(id, n->r);
        }
    }
    // 찾는 환자 번호 < 현재 노드 환자 번호
    else {
        // 현재 노드의 왼쪽 자식이 leaf노드이면 -> 환자 존재 X
        if (*n->l->id == -1) {
            cout << "Not found" << endl;
        }
        // 현재 노드의 왼쪽 자식이 internal노드이면
        else {
            // 현재 노드를 왼쪽 자식으로 바꾸어주고, 검색 메서드 재귀수행
            search(id, n->l);
        }
    }
}

// 추가 진료 메서드
void RBT::addForSearch(int id, string DI, int C, node* n) {
    
    // 환자 검색 성공, 질병 및 진료비 처리
    if (id == *n->id) {
        n->disease.push_back(DI);
        *n->pay += C;
        flag = true;
    }
    // 찾는 환자 번호 > 현재 노드 환자 번호
    else if (id > * n->id) {
        // 현재 노드의 오른쪽 자식이 leaf노드이면 -> 환자 존재 X
        if (*n->r->id == -1) {
            cout << "Not found" << endl;
        }
        // 현재 노드의 오른쪽 자식이 internal노드이면
        else {
            // 현재 노드를 오른쪽 자식으로 바꾸어주고, 추가 진료 메서드 재귀수행
            addForSearch(id, DI, C, n->r);
        }
    }
    // 찾는 환자 번호 < 현재 노드 환자 번호
    else {
        // 현재 노드의 왼쪽 자식이 leaf노드이면 -> 환자 존재 X
        if (*n->l->id == -1) {
            cout << "Not found" << endl;
        }
        // 현재 노드의 왼쪽 자식이 internal노드이면
        else {
            // 현재 노드를 왼쪽 자식으로 바꾸어주고, 추가 진료 메서드 재귀수행
            addForSearch(id, DI, C, n->l);
        }
    }
}

// 유행병 조사 메서드
void RBT::epiForSearch(string DI) {
    int T = 0;

    // node list를 통해 전체 노드를 조사, 최근 질병이 입력값과 같으면 T값 증가 후 최종적으로 T값 출력
    for (unsigned int i = 0; i < nodelist.size(); i++) {
        if (nodelist[i]->disease[nodelist[i]->disease.size() - 1] == DI) {
            T++;
        }
    }
    cout << T << endl;
}

// 노드의 깊이와 정보를 출력하는 메서드(전역변수 depth 사용)
void RBT::printDepthAndInfo(int id, node* n) {

    // 해당 노드 발견, 깊이와 정보 출력
    if (id == *n->id) {
        cout << depth << " " << *n->name << " " << *n->num << " " << *n->add_x << " " << *n->add_y << endl;

        // 전역변수 초기화
        depth = 0;
    }
    // 찾는 노드 번호 > 현재 노드 번호
    else if (id > *n->id) {
        // depth 증가 후 재귀수행
        depth++;
        printDepthAndInfo(id, n->r);
    }
    // 찾는 노드 번호 < 현재 노드 번호
    else {
        // depth 증가 후 재귀수행
        depth++;
        printDepthAndInfo(id, n->l);

    }
}

// 노드의 깊이를 반환해주는 메서드(전역변수 depth 사용)
int RBT::returnDepth(int id, node* n) {

    // 해당 노드 발견, 깊이 값 반환
    if (id == *n->id) {
        int result = depth;
        depth = 0;
        return result;
    }
    // 찾는 노드 번호 > 현재 노드 번호
    else if (id > *n->id) {
        // depth 증가 후 재귀수행
        depth++;
        returnDepth(id, n->r);
    }
    // 찾는 노드 번호 < 현재 노드 번호
    else {
        // depth 증가 후 재귀수행
        depth++;
        returnDepth(id, n->l);

    }
}