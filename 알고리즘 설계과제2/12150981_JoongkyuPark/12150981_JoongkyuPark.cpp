#include <iostream>
#include <vector>
#include <cmath>
#include <stack>
#include <queue>

using namespace std;

// 정점의 상태를 define으로 정의
#define UNSEEN  0
#define FRINGE  1
#define TREE  2

// 최대 숫자(infinity)
int maximumNum = 2147483646;

class vertex;

// 정점들을 담을 vertices list
vector<vertex*> vList;

// Tree vertex 갯수 변수
int N = 0;

// vertex 클래스
class vertex {
public:
    // 정점 정보
    int id;
    string name;
    bool isFlooded;
    vector<pair<vertex*, int>> adjacentVertices;

    // 정점 상태, 목적지까지 거리, predecessor 변수
    int state = UNSEEN;
    int dToSource = maximumNum;
    vertex* pred = nullptr;

    vertex(int _id, string _name, bool _isFlooded) {
        this->id = _id;
        this->name = _name;
        this->isFlooded = _isFlooded;
    }

    // 인접정점들을 추가하는 메소드
    void insertAdjacentvertex(vertex* v, int weight) {
        this->adjacentVertices.push_back(make_pair(v, weight));
    }
};

// 다익스트라 함수의 priority queue에서 사용하는 비교연산. 목적지까지의 거리를 비교하여 작은게 우선순위가 높음(거리가 같다면 id가 작은게 우선순위가 높음)
struct compareForDistance {
    bool operator()(vertex* a, vertex* b) {
        if (a->dToSource == b->dToSource) {
            return a->id > b->id;
        }
        else {
            return a->dToSource > b->dToSource;
        }
    }
};

// 바이너리 서치를 이용한 정점찾기 메소드
vertex* findVertex(int id) {
    int size = vList.size();
    int start = 0;
    int end = size - 1;
    int mid;

    while (end - start >= 0) {
        mid = (start + end) / 2;

        if (vList[mid]->id == id) {
            return vList[mid];
        }
        else if (vList[mid]->id > id) {
            end = mid - 1;
        }
        else {
            start = mid + 1;
        }
    }
    return nullptr;
}

// 최단거리를 업데이트 해주는 메소드
bool decreasekey(vertex* t, vertex* f, int weight) {
    if (f->dToSource > t->dToSource + weight) {
        f->dToSource = t->dToSource + weight;
        f->pred = t;
        return true;
    }
    else
        return false;
}


// 다익스트라 함수
void Dijkstra(vertex* s, vertex* d) {

    // 매 loop마다 목적지까지의 거리가 가장 짧은걸 뽑기위한 priority queue
    priority_queue<vertex*, vector<vertex*>, compareForDistance> pq;

    // 정점 초기화(predecessor, state, 목적지까지의 거리)
    for (unsigned int i = 0; i < vList.size();i++) {
        vList[i]->pred = nullptr;
        vList[i]->state = UNSEEN;
        vList[i]->dToSource = maximumNum;
    }
    // 출발점의 거리 정보 0으로 초기화
    s->dToSource = 0;

    // priority queue에 출발정점 삽입
    pq.push(s);

    // priority queue가 빌 때 까지 루프반복
    while (!pq.empty()) {
        // 목적지까지의 거리가 가장 짧은 정점 t
        vertex* t = pq.top();
        pq.pop();

        // t의 state가 이미 TREE면 넘긴다
        if (t->state != TREE) {

            // t의 state를 TREE로 바꾸어준후, N값 1 증가
            t->state = TREE;
            N++;
            
            // 만약 t가 목적지라면 함수종료
            if (t->id == d->id) {
                break;
            }

            // t의 인접정점들 확인
            for (unsigned int i = 0; i < t->adjacentVertices.size();i++) {
                // t의 인접정점의 state가 FRINGE이면
                if (t->adjacentVertices[i].first->state == FRINGE) {
                    // decreaskey를 수행해보고, 업데이트가 가능하면 거리 업데이트 후 priority queue에 인접정점 삽입 
                    if (decreasekey(t, t->adjacentVertices[i].first, t->adjacentVertices[i].second))
                        pq.push(t->adjacentVertices[i].first);
                }
                // t의 인접정점의 state가 UNSEEN이고 침수가 안된 지역이라면
                else if (t->adjacentVertices[i].first->state == UNSEEN && t->adjacentVertices[i].first->isFlooded == 0) {
                    // 인접정점의 state를 FRINGE로 바꾸고, decreasekey 수행 후 priority queue에 인접정점 삽입
                    t->adjacentVertices[i].first->state = FRINGE;
                    if (decreasekey(t, t->adjacentVertices[i].first, t->adjacentVertices[i].second))
                        pq.push(t->adjacentVertices[i].first);
                }
            }
        }
    }
}

// 최단경로를 출력하는 함수
void printPath(int sourceId, vertex* d) {
    // 스택 자료구조에 목적지 정점부터 시작하여 출발점 정점까지의 모든 정점을 담는다
    stack<vertex*> s;
    s.push(d);
    vertex* v = d->pred;
    while (1) {
        s.push(v);

        // 출발정점일 때 루프탈출
        if (v->id == sourceId)
            break;

        v = v->pred;

    }

    // 스택에서 하나씩 꺼내며 id값 출력
    while (!s.empty()) {
        cout << s.top()->id << " ";
        s.pop();
    }
    cout << "\n";
}


// vertices list를 id순으로 정렬하기 위한 함수
struct compareForInsert {
    bool operator()(vertex* a, vertex* b) {
        return a->id > b->id;
    }
};

int main()
{
    int n;
    int m;
    int q;

    // vertices list를 id순으로 정렬하기 위해 임시로 쓰이는 priority queue
    priority_queue<vertex*, vector<vertex*>, compareForInsert> pq_vList;

    cin >> n >> m >> q;

    // 정점정보 입력
    while (n--) {
        int id;
        string name;
        int isFlooded;

        cin >> id >> name >> isFlooded;

        vertex* v = new vertex(id, name, isFlooded);

        // priority queue에 id가 작은 것이 우선순위가 높게 삽입
        pq_vList.push(v);
    }

    // id가 작은 것 부터 빼면서 vList에 id순으로 정점삽입(findVertex 함수를 위함)
    while (!pq_vList.empty()) {
        vertex* v = pq_vList.top();
        vList.push_back(v);

        pq_vList.pop();
    }

    // 간선정보 입력
    while (m--) {
        int vid1 = 0;
        int vid2 = 0;
        int weight = 0;

        cin >> vid1 >> vid2 >> weight;

        vertex* v1 = findVertex(vid1);
        vertex* v2 = findVertex(vid2);

        v1->insertAdjacentvertex(v2, weight);
        v2->insertAdjacentvertex(v1, weight);
    }

    // 질의수행
    while (q--) {
        char query;
        cin >> query;

        int sid;
        int did;
        cin >> sid >> did;

        N = 0;

        // 질의 A
        if (query == 'A') {

            // id에 해당하는 두 정점찾기
            vertex* s = findVertex(sid);
            vertex* d = findVertex(did);

            // 출발지 또는 목적지가 침수지역일 때
            if (s->isFlooded == 1 || d->isFlooded == 1) {
                cout << "None" << endl;
            }
            else {
                // 다익스트라 함수로 목적지까지의 최단거리, 최단경로 업데이트
                Dijkstra(s, d);

                // 최단거리가 10^6이 넘어갈 때
                if (d->dToSource > pow(10, 6)) {
                    cout << "None" << endl;
                }
                // Tree 정점 수와 최단거리 및 출발점, 도착점 이름 출력
                else {
                    cout << N << " " << d->dToSource << " " << s->name << " " << d->name << endl;
                }

            }
        }
        // 질의 B
        else {

            // id에 해당하는 두 정점찾기
            vertex* s = findVertex(sid);
            vertex* d = findVertex(did);

            // 출발지 또는 목적지가 침수지역일 때
            if (s->isFlooded == 1 || d->isFlooded == 1) {
                cout << "None" << endl;
            }           
            else {
                // 다익스트라 함수로 목적지까지의 최단거리, 최단경로 업데이트
                Dijkstra(s, d);

                // 최단거리가 10^6이 넘어갈 때
                if (d->dToSource > pow(10, 6)) {
                    cout << "None" << endl;
                }
                else {
                    // Tree 정점 수와 최단경로 출력
                    cout << N << " ";
                    if (s->id == d->id) {
                        cout << s->id << endl;
                    }
                    else {
                        printPath(sid, d);
                    }
                }
            }
        }
    }
}