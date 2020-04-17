#include "UCT.h"
#include <math.h>
#include <stdlib.h>
UCT::UCT() {
    child = new int[N];
    for (int i = 0; i < N; i++) {
        child[i] = -1;
    }
}

UCT::~UCT() {
    delete[]child;
}

bool UCT::Scalable() {
    bool NotVisited = false;
    for (int i = 0; i < N; i++) {
        if (child[i] == -1) {
            NotVisited = true;
            break;
        }
    }
    return NotVisited;
}

bool UCT::Finished() {
    return terminal;
}

UCT* UCT::BestChild(const int M, const int N, int** board, int* top) {
    int BestID = -1;
    int BestLine = -1;
    double BestUCTValue = -1000000.0;
    for (int i = 0; i < N; i++) {
        double nowUCT = node[child[i]].getUCTValue();
        if (nowUCT > BestUCTValue) {
            BestUCTValue = nowUCT;
            BestID = child[i]; 
            BestLine = i;
        }
    }
    // TODO NOT PUTABLE LINE, (FULL OR NO!!!!)
    PlaceChess(BestLine, M, N, board, top);
    return &node[BestID];
}

void UCT::PlaceChess(int line, const int M, const int N, int** board, int* top) {
    int chess = -1;
    if (id == 1) {  // me
        chess = 2;
    } else {
         chess = 1;
    }
    top[line] -= 1;
    board[top[line]][line] = chess;
}

double UCT::getUCTValue() {
    double winRatio = win / (total + 0.001);
    double explore = sqrt(log(node[father].total + 1.001) / (total + 0.001));
    return (winRatio + coefficient * explore);
}
void UCT::shuffule(int* array, int n) {
    srand(unsigned(time(NULL)));
    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

UCT* UCT::Expand(const int M, const int N, int** board, int* top) {
    int* tmp = new int[N];
    int unDiscovered = 0;
    for (int i = 0; i < N; i++) {
        if (child[i] == -1) {
            tmp[unDiscovered++] = i;
        }
    }
    shuffle(tmp, unDiscovered);
    // create new node with tmp[0]
    // TODO NOT PUTABLE LINE, (FULL OR NO!!!!)
    PlaceChess(tmp[0], M, N, board, top);
    delete[]tmp;
}

