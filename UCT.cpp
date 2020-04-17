#include "UCT.h"
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
using namespace std;
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
bool UCT::Scalable(int* top) {
    bool NotVisited = false;
    for (int i = 0; i < N; i++) {
        if (child[i] == -1 && top[i] != 0) {
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
        if (nowUCT > BestUCTValue && top[i] != 0) {
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
    if (top[line] < 0) {
        fprintf(stderr, "want to place on %d %d, program exit with code0\n", top[line], line);
        exit(0);
    }
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
    // TODO is it ok here?
    for (int i = 0; i < N; i++) {
        if (child[i] == -1) {
            if (top[i] != 0)
                tmp[unDiscovered++] = i;
        }
    }


    if (unDiscovered == 1) {
        scalable = false;
    }
    // for (int i =0 ; i < N;i++) {
    //     printf("node %d child[%d]=%d ", index, i, child[i]);
    // }
    // printf("\n");
    shuffle(tmp, unDiscovered);

    // for (int i = 0; i < unDiscovered; i++) {
    //     printf("tmp[%d]=%d\n", i, tmp[i]);
    // }
    // printf("\n");
    // printf("expanding %d from %d\n", tmp[0], index);
    // create new node with tmp[0]
    // TODO NOT PUTABLE LINE, (FULL OR NO!!!!)
    // printf("before placing chess on %d %d\n", top[tmp[0]] - 1, tmp[0]);
    // if (top[tmp[0]] == 0) {
    //     for (int i = 0; i < M; i++) {
    //         for (int j = 0; j < N; j++) {
    //             printf("%d ", board[i][j]);
    //         }
    //         printf("\n");
    //     }
    // }
    int selectedLine = tmp[0];
    // printf("want to place on %d %d\n", top[selectedLine] - 1, selectedLine);
    PlaceChess(selectedLine, M, N, board, top);
    // printf("after placing chess\n");
    child[selectedLine] = cnt++;
    node[child[selectedLine]].index = cnt - 1;
    node[child[selectedLine]].father = index;
    node[child[selectedLine]].id = (1 - (int)(id));
    // modify terminal, scalable
    node[child[selectedLine]].terminal = nowWin(M, N, board, top[selectedLine], selectedLine);
    // printf("tmp[0] == %d, cnt - 1 = %d, child[tmp[0]] = %d, id == %d\n", tmp[0], cnt - 1, child[tmp[0]], node[child[tmp[0]]].getID());
    delete[]tmp;

    return (&(node[child[selectedLine]]));
}

int UCT::BestChildAction() {
    int BestAction = -1;
	double BestWinRatio = -0x3f3f3f3f;
    // printf("my status win == %d total == %d\n", win, total);
    // printf("children == \n");
    // for (int i = 0; i < N; i++) {
    //     printf("%d ", child[i]);
    // }
    // printf("\n");
	for (int i = 0; i < N; i++) {
		double nowRatio = node[child[i]].getWinRatio();
        // printf("line[%d]", i);
        printf("line[%d] win ratio = %f\n", i, nowRatio);
        if (nowRatio > BestWinRatio) {
            BestWinRatio = nowRatio;
            BestAction = i;
        }
	}

    return BestAction;
}

double UCT::getWinRatio() {
    // printf("win == %d, total == %d\n", win, total);
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
int UCT::getFather() {
    return father;
}
void UCT::modify(int profit) {
    total += 1;
    if (id == 1) {
        win += profit;
    } else {
        win -= profit;
    }
    
}
int nowWin(int M, int N, int** board, int x, int y) {
	// row
	for (int i = 0; i + 4 <= N; i++) {
		bool findIt = true;
		for (int j = i; j < i + 4; j++) {
			if (board[x][j] != board[x][y]) {
				findIt = false;
				break;
			}
		}
		if (findIt) {
			return 1;
		}
	}
	
	// column
	for (int i = 0; i + 4 <= M; i++) {
		bool findIt = true;
		for (int j = i; j < i + 4; j++) {
			if (board[j][y] != board[x][y]) {
				findIt = false;
				break;
			}
		}
		if (findIt) {
			return 1;
		}
	}

	// down left to up right	
	for (int i = 0; i < 4; i++) {
		if (x + i - 3 >= 0 && y + (3 - i) < N && x + i < M && y - i >= 0) {
			bool findIt = 1;
			for (int s = x + i, t = y - i; s >= x + i - 3 && t <= y + 3 - i; s--, t++) {
				if (board[s][t] != board[x][y]) {
					findIt = 0;
					break;
				}
			}
			if (findIt) {
				return 1;
			}
		}
	}

	// up left to down right
	for (int i = 0; i < 4; i++) {
		if (x - i >= 0 && y + (3 - i) < N && x - i + 3 < M && y - i >= 0) {
			bool findIt = 1;
			for (int s = x - i, t = y - i; s <= x - i + 3 && t <= y + 3 - i; s++, t++) {
				if (board[s][t] != board[x][y]) {
					findIt = 0;
					break;
				}
			}
			if (findIt) {
				return 1;
			}
		}
	}

	return 0;
}

