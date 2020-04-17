// Class definition for UCT
#ifndef  __UCT_H_
#define   __UCT_H_
class UCT {
    bool scalable = true;
    bool id = 1;  //  my id == 1, component id == 0
    bool terminal = false;
    int index = 0;
    int father = -1;
    int* child;  // is new child necessary for all nodes? if not, you should change the way to access child.
    int win = 0;
    int total = 0;
    void shuffle(int*, int);
    double getWinRatio();
public:
    static const int MAX_NODES = 10000000;
    static UCT* node;
    static int N;
    static int cnt;
    static double coefficient;

    UCT();
    ~UCT();
    bool Scalable(int* top);
    bool Finished();
    bool getID();
    bool isRoot();
    int getFather();
    void modify(int);
    UCT* BestChild(const int M, const int N, int** board, int* top);
    double getUCTValue();
    int BestChildAction();
    void PlaceChess(int line, const int M, const int N, int** board, int* top);
    UCT* Expand(const int M, const int N, int** board, int* top);
};

int nowWin(int M, int N, int** board, int x, int y);

#endif