// Class definition for UCT

class UCT {
    bool scalable = false;
    bool id = 0;  //  my id == 1, component id == 0
    bool terminal = false;
    int father = -1;
    int* child;
    int win = 0;
    int total = 0;
    static const double coefficient = 1.0;
    void shuffule(int*, int);
    void PlaceChess(int line, const int M, const int N, int** board, int* top);
public:
    static const int MAX_NODES = 10000000;
    static UCT* node;
    static int N;
    UCT();
    ~UCT();
    bool Scalable();
    bool Finished();
    UCT* BestChild(const int M, const int N, int** board, int* top);
    double getUCTValue();
    UCT* Expand(const int M, const int N, int** board, int* top);
};
