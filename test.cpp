#include <iostream>


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
class A {
public:
	static int a;
	int* get() {
		return &a;
	}
};
int A::a;
int main() {
    // int M = 10;
    // int N = 12;
    // int** board = new int*[M];
    // for (int i = 0; i < 10; i++) {
    //     board[i] = new int[N];
    // }
    // for (int i = 0; i < M; i++) {
    //     for (int j = 0; j < N; j++) {
    //         scanf("%d", &board[i][j]);
    //     }
    // }
    // int x, y;
    // scanf("%d%d", &x, &y);
    // int res = nowWin(M, N, board, x, y);
    // printf("%d\n", res);
	A::a = 1;
	A k;
	printf("%d\n", *k.get());
    return 0;
}
