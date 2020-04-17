#include "UCT.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
UCT::UCT() {
    child = new int[N];
    for (int i = 0; i < N; i++) {
        child[i] = -1;
    }
}

UCT::~UCT() {
    delete[]child;
}

// TODO is scalable directly callable?
bool UCT::Scalable() {
    bool NotVisited = false;
    for (int i = 0; i < N; i++) {
        if (child[i] == -1) {
            NotVisited = true;
            break;
        }
    }
    return NotVisited;
    // return scalable;
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
void UCT::shuffle(int* array, int n) {
    if (n > 1) {
        srand(unsigned(time(NULL)));
        for (int i = 0; i < n - 1; i++) {
          int j = i + rand() / (RAND_MAX / (n - i) + 1);
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

    if (unDiscovered == 1) {
        scalable = false;
    }
    shuffle(tmp, unDiscovered);
    // create new node with tmp[0]
    // TODO NOT PUTABLE LINE, (FULL OR NO!!!!)
    PlaceChess(tmp[0], M, N, board, top);
    child[tmp[0]] = cnt++;
    node[child[tmp[0]]].index = cnt - 1;
    node[child[tmp[0]]].father = index;
    node[child[tmp[0]]].id = (1 - (int)(id));
    // modify terminal, scalable
    node[child[tmp[0]]].terminal = Terminated(tmp[0], M, N, board, top);

    delete[]tmp;
    return &node[child[tmp[0]]];
}

int UCT::BestChildAction() {
    int BestAction = -1;
	double BestWinRatio = -1;
	for (int i = 0; i < N; i++) {
		double nowRatio = node[child[i]].getWinRatio();
        if (nowRatio > BestWinRatio) {
            BestWinRatio = nowRatio;
            BestAction = i;
        }
	}
    return BestAction;
}

double UCT::getWinRatio() {
    return win / (total + 0.001);
}

bool UCT::getID() {
    return id;
}

bool UCT::isRoot() {
    if (father == -1) {
        return true;
    }
    return false;
}

void UCT::modify(int profit) {
    total++;
    if (id == 1) {
        win += profit;
    } else {
        win -= profit;
    }
    
}
