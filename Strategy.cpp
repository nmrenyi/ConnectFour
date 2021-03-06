#include <iostream>
#include <unistd.h>
#include "Point.h"
#include <time.h>
#include "UCT.h"
#include "Strategy.h"
#include "UCT.cpp"
using namespace std;


int cnt = 0; // counter for nodes
clock_t startTime = 0;
const double LIMIT = 2.0;
void my_assert(bool ok, const char* prompt);
UCT* UCT::node;
int UCT::N;
int UCT::cnt;
double UCT::coefficient;
void BackUp(UCT* now, int profit);
int DefaultPolicy(const int M, const int N, int** board, int* top, int id);
void ResetBoard(const int M, const int N, int** board, const int* _board, int* top, const int* _top);
UCT* TreePolicy(UCT* root, const int M, const int N, int** board, int* top);
int UCT_Search(const int M, const int N, int** board, const int* _board, int* top, const int* _top);
/*
	策略函数接口,该函数被对抗平台调用,每次传入当前状态,要求输出你的落子点,该落子点必须是一个符合游戏规则的落子点,不然对抗平台会直接认为你的程序有误
	
	input:
		为了防止对对抗平台维护的数据造成更改，所有传入的参数均为const属性
		M, N : 棋盘大小 M - 行数 N - 列数 均从0开始计， 左上角为坐标原点，行用x标记，列用y标记
		top : 当前棋盘每一列列顶的实际位置. e.g. 第i列为空,则_top[i] == M, 第i列已满,则_top[i] == 0
		_board : 棋盘的一维数组表示, 为了方便使用，在该函数刚开始处，我们已经将其转化为了二维数组board
				你只需直接使用board即可，左上角为坐标原点，数组从[0][0]开始计(不是[1][1])
				board[x][y]表示第x行、第y列的点(从0开始计)
				board[x][y] == 0/1/2 分别对应(x,y)处 无落子/有用户的子/有程序的子,不可落子点处的值也为0
		lastX, lastY : 对方上一次落子的位置, 你可能不需要该参数，也可能需要的不仅仅是对方一步的
				落子位置，这时你可以在自己的程序中记录对方连续多步的落子位置，这完全取决于你自己的策略
		noX, noY : 棋盘上的不可落子点(注:涫嫡饫锔?龅膖op已经替你处理了不可落子点，也就是说如果某一步
				所落的子的上面恰是不可落子点，那么UI工程中的代码就已经将该列的top值又进行了一次减一操作，
				所以在你的代码中也可以根本不使用noX和noY这两个参数，完全认为top数组就是当前每列的顶部即可,
				当然如果你想使用lastX,lastY参数，有可能就要同时考虑noX和noY了)
		以上参数实际上包含了当前状态(M N _top _board)以及历史信息(lastX lastY),你要做的就是在这些信息下给出尽可能明智的落子点
	output:
		你的落子点Point
*/
extern "C" Point *getPoint(const int M, const int N, const int *top, const int *_board,
						   const int lastX, const int lastY, const int noX, const int noY)
{
	startTime = clock();
	/*
		不要更改这段代码
	*/

	int **board = new int *[M];
	for (int i = 0; i < M; i++)
	{
		board[i] = new int[N];
		for (int j = 0; j < N; j++)
		{
			board[i][j] = _board[i * N + j];
		}
	}

	// 一共M行，N列
	// x为行号，y为列号

	/*
		根据你自己的策略来返回落子点,也就是根据你的策略完成对x,y的赋值
		该部分对参数使用没有限制，为了方便实现，你可以定义自己新的类、.h文件、.cpp文件
	*/
	//Add your own code below
	int x = -1, y = -1; //最终将你的落子点存到x,y中
	
    //  //a naive example
	// for (int i = N-1; i >= 0; i--) {
	// 	if (top[i] > 0) {
	// 		x = top[i] - 1;
	// 		y = i;
	// 		break;
	// 	}
	// }
    
	// TODO constructor for UCT, needs N
	UCT::N = N;
	UCT::coefficient = 1.0;
	UCT::cnt = 0;
	UCT::node = new UCT[UCT::MAX_NODES];
	int* my_top = new int[N];
	for (int i = 0; i < N; i++) {
		my_top[i] = top[i];
	}
	// UCT_Search(M, N, board, _board, my_top, top);
	y = UCT_Search(M, N, board, _board, my_top, top);
	x = top[y] - 1;
	fprintf(stderr, "want %d %d\n", x, y);
	delete []UCT::node;
	return new Point(x, y);
}

int UCT_Search(const int M, const int N, int** board, const int* _board, int* top, const int* _top) {
	UCT* root = &UCT::node[UCT::cnt++];
	// int cntt = 0;
	while (UCT::cnt < UCT::MAX_NODES && ((double)(clock() - startTime) / CLOCKS_PER_SEC) < LIMIT) {
		// cntt ++;
		// printf("entering tree policy\n");
		UCT* NowNode = TreePolicy(root, M, N, board, top);
		// printf("out of tree policy\n");
		// TODO CHECK if I win, profit == -1, if I lose, profit == -1;
		// cout << NowNode << endl;
		// printf("id == %d\n", NowNode->getID());
		// printf("in default policy\n");
		
		int profit = DefaultPolicy(M, N, board, top,(int)(NowNode->getID()));
		// printf("out of default policy\n");
		// printf("in back up\n");
		BackUp(NowNode, profit);
		// printf("out of back up\n");
		ResetBoard(M, N, board, _board, top, _top);
	}
	
	return root->BestChildAction();
}

void ResetBoard(const int M, const int N, int** board, const int* _board, int* top, const int* _top) {
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			board[i][j] = _board[i * N + j];
		}
	}
	for (int i = 0; i < N; i++) {
		top[i] = _top[i];
	}
}


UCT* TreePolicy(UCT* root, const int M, const int N, int** board, int* top) {
	UCT* now = root;
	while (!now->Finished()) {
		// printf("here\n");
		if (now->Scalable(top)) {
			// printf("in expand!!!!!!!!!!!\n");
			UCT* ret = now->Expand(M, N, board, top);
			// cout << ret ;
			// printf(" aoh\n");
			// printf("id == %d\n", ret->getID());
			// printf("out of expand!!\n");
			return ret;
		} else {
			// printf("selecting best\n");
			now = now->BestChild(M, N, board, top);
		}
	}
	return now;
}

void BackUp(UCT* now, int profit) {
	while (true) {
		now->modify(profit);
		if (now->isRoot()) {
			break;
		}
		now = &UCT::node[now->getFather()];
	}
	// while (!now->isRoot()) {
	// 	now->modify(profit);
	// 	now = &UCT::node[now->getFather()];
	// }
}

void shuffleArr(int* array, int n) {
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

int RandomAction(int* top, int N, int* tmp) {
	int availableAction = 0;
	for (int i = 0; i < N; i++) {
		if (top[i] != 0) {
			tmp[availableAction++] = i;
		}
	}
	if (availableAction == 0) {
		return -1;
	}
	shuffleArr(tmp, availableAction);
	return tmp[0];
}

void PutChess(int line, const int M, const int N, int** board, int* top, int id) {
    int chess = -1;
    if (id == 1) {  // me
        chess = 2;
    } else {
         chess = 1;
    }
    top[line] -= 1;
    board[top[line]][line] = chess;
}

int DefaultPolicy(const int M, const int N, int** board, int* top, int id) {
	// int winnerID = 0;
	// int lastX = -1, lastY = -1;
	// printf("entering default\n");
	int* tmp = new int[N];
	int profit = 0;
	while (true) {
		// printf("getting radom act\n");
		int randomChoice = RandomAction(top, N, tmp);
		// printf("get complete\n");
		if (randomChoice == -1) {
			profit = 0;  // tie
			break;
		}
		// printf("want to put on %d %d\n", top[randomChoice] - 1, randomChoice);
		PutChess(randomChoice, M, N, board, top, id);
		if (nowWin(M, N, board, top[randomChoice], randomChoice)) {
			if (id == 1) {
				profit = 1;
				break;
			} else {
				profit = -1;
				break;
			}
		}
		id = 1 - id;
	}

	delete[]tmp;
	return profit;  // tie, need an assert?
}

/*
	getPoint函数返回的Point指针是在本so模块中声明的，为避免产生堆错误，应在外部调用本so中的
	函数来释放空间，而不应该在外部直接delete
*/
extern "C" void clearPoint(Point *p)
{
	delete p;
	return;
}

/*
	清除top和board数组
*/
void clearArray(int M, int N, int **board)
{
	for (int i = 0; i < M; i++)
	{
		delete[] board[i];
	}
	delete[] board;
}

void my_assert(bool ok, const char* prompt) {
	if (!ok) {
		fprintf(stderr, "%s, program exit with code 0\n", prompt);
		exit(0);
	}
}
