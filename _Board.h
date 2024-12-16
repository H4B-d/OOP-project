#pragma once
#include "_Point.h"
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include "_Common.h"
#include <random>
#include <time.h>
#include <algorithm>

#define MAX_DEPTH 3

using namespace std;

struct Move
{
	int row , col;
};

class _Board
{
private:
	int _size;
	int _left, _top;
	int _currentSize;
	_Point** _pArr;
public:
    int getSize();
    int getLeft();
    int getTop();
    int getXAt(int, int);
    int getYAt(int, int);
    void resetData();
    void drawBoard();
    void setSize(int);
    int checkBoard(int, int, bool);
    int testBoard(int&, int&);
    int Undo(int, int);
    int GetXO(int, int);
    void Init(int, int, int);
    int checkBoardCPU(int&, int&, bool);
    void CPU(int&, int&);
    void Minimax(int&, int&);
    void AlphaBeta(int&, int&);

    long long TC_Dong(int, int);
    long long TC_Cot(int, int);
    long long TC_CheoXuoi(int, int);
    long long TC_CheoNguoc(int, int);
    
    long long PT_Dong(int, int);
    long long PT_Cot(int, int);
    long long PT_CheoXuoi(int, int);
    long long PT_CheoNguoc(int, int);

    bool isMovesLeft();
    
	int evaluate();

    // Minimax
    int minimax(int depth, bool isMax);
    Move findBestMoveMinimax();

    // Alpha-Beta Pruning
    int alphabeta(int depth, bool isMax, int alpha, int beta);
    Move findBestMoveAlphaBeta();

public:
    _Board();
    _Board(int, int, int);
    ~_Board();
};



long long attack[7] = { 0, 9, 54, 162, 1458, 13112, 118008 };
long long defense[7] = { 0, 3, 27, 99, 729, 6561, 59049 };


int _Board::getSize(){ return _size; }

int _Board::getLeft(){ return _left; }

int _Board::getTop(){ return _top; }

int _Board::getXAt(int i, int j) { return _pArr[i][j].getX(); }

int _Board::getYAt(int i, int j) { return _pArr[i][j].getY(); }

_Board::_Board()
{
	_size = 0;
	_left = 0;
	_top = 0;
	_pArr = NULL;
}

_Board::_Board(int pSize, int pX, int pY)
{
	_size = pSize;
	_left = pX;
	_top = pY;
	_pArr = new _Point*[pSize];
	for (int i = 0; i < pSize; i++) _pArr[i] = new _Point[pSize];
}

_Board::~_Board()
{

	for (int i = 0; i < _size; i++) delete[] _pArr[i];
		delete[] _pArr;
}

void _Board::resetData() 
{
	if (_size == 0) return; // Phải gọi constructor trước khi resetData
	for (int i = 0; i < _size; i++)
	{
		for (int j = 0; j < _size; j++)
		{
			_pArr[i][j].setX(4 * j + _left + 2); // Trùng với hoành độ màn hình bàn cờ
			_pArr[i][j].setY(2 * i + _top + 1); // Trùng với tung độ màn hình bàn cờ
			_pArr[i][j].setCheck(0);
		}
	}
}

void _Board::Init(int i,int j, int value)
{
	if (_pArr == NULL) return; // Phải gọi constructor trước khi resetData

	_pArr[i][j].setX(4 * j + _left + 2); // Trùng với hoành độ màn hình bàn cờ
	_pArr[i][j].setY(2 * i + _top + 1); // Trùng với tung độ màn hình bàn cờ
	_pArr[i][j].setCheck(value);
}

void _Board::drawBoard()
{
	if (_size == 0 ) return; // phải gọi constructor trước

	_Common::gotoXY(0, 0);
	for (int i = 0; i <= _left + 4 * _size + 1; i++)
	{
		if (i == 0) putchar(201);
		else if (i == _left + 4 * _size + 1) putchar(187);
		else putchar(205);
		
	}
	for (int i = 1; i <= _top + 2 * _size; i++)
	{
		_Common::gotoXY(0, i); putchar(186);
		_Common::gotoXY(_left + 4 * _size + 1, i); putchar(186);
	}
	_Common::gotoXY(0, _top + 2 * _size + 1);
	for (int i = 0; i <= _left + 4 * _size + 1; i++)
	{
		if (i == 0) putchar(200);
		else if (i == _left + 4 * _size + 1) putchar(188);
		else putchar(205);

	}
	for (int i = 0; i <= _size; i++)
	{
		for (int j = 0; j <= _size; j++)
		{
			_Common::gotoXY(_left + 4 * i, _top + 2 * j);
			printf(".");
		}
	}

	// thiet ke giao dien
	_Common::gotoXY(_left + 4 * _size + 17, 8);
	cout << "F1 : New Game";
	_Common::gotoXY(_left + 4 * _size + 17, 10);
	cout << "F2 : Save Game";
	_Common::gotoXY(_left + 4 * _size + 37, 8);
	cout << "F3 : Load Game";
	_Common::gotoXY(_left + 4 * _size + 37, 10);
	cout << "F4 : Undo Step";
	_Common::gotoXY(_left + 4 * _size + 26, 12);
	cout << "ESC: Return Menu!";

	_Common::gotoXY(_pArr[0][0].getX(), _pArr[0][0].getY());// di chuyển vào ô đầu
}


 void _Board::setSize(int size) {
        _size = size;
        resetData();  // Cập nhật lại dữ liệu bàn cờ khi thay đổi kích thước
        drawBoard();  // Vẽ lại bàn cờ với kích thước mới
    }

int _Board::GetXO(int x, int y)
{
	return _pArr[x][y].getCheck();
}


int _Board::checkBoard(int pX, int pY, bool pTurn)
{
	for (int i = 0; i < _size; i++)
	{
		for (int j = 0; j < _size; j++)
		{
			if (_pArr[i][j].getX() == pX && _pArr[i][j].getY() == pY && _pArr[i][j].getCheck() == 0)
			{
				if (pTurn) _pArr[i][j].setCheck(-1);// Nếu lượt hiện hành là true: c = -1
				else _pArr[i][j].setCheck(1);// Nếu lượt hiện hành là false: c = 1
				return _pArr[i][j].getCheck();
			}
		}
	}
	return 0;
}



int _Board::Undo(int pX, int pY)
{
	int res;
	for (int i = 0; i < _size; i++)
	{
		for (int j = 0; j < _size; j++)
		{
			if (_pArr[i][j].getX() == pX && _pArr[i][j].getY() == pY && _pArr[i][j] .getCheck()!= 0)
			{
				res = _pArr[i][j].getCheck();
				_pArr[i][j].setCheck(0);
				return res;
			}
		}
	}
	return 0;
}

int _Board::testBoard(int& pX,int& pY)
{
	// Xu ly hoa
	bool check = true;
	for (int i = 0; i < _size; i++)
	{
		for (int j = 0; j < _size; j++)
		{
			if (_pArr[i][j].getCheck() == 0)
			{
				check = false;
				break;
			}
		}
	}
	if (check) return 0;

	// Xu ly thang/thua
	int x, y = 0;

	for (int i = 0; i < _size; i++)
	{
		for (int j = 0; j < _size; j++)
		{
			if (_pArr[i][j].getX() == pX && _pArr[i][j].getY() == pY)
			{
				x = i;
				y = j;
				break;
			}
		}
	}

	int res = 0;
	int xStart = (x - 4 > 0) ? x - 4 : 0;
	int yStart = (y - 4 > 0) ? y - 4 : 0;

	int xEnd = (x + 4 >= _size) ? _size - 1 : x + 4;
	int yEnd = (y + 4 >= _size) ? _size - 1 : y + 4;

	int x1, x2, y1, y2;

	for (int i = xStart; i <= x; i++) {
    	if (i + 4 < _size) {
        	res = _pArr[i][y].getCheck() + _pArr[i + 1][y].getCheck() + _pArr[i + 2][y].getCheck() +
              	_pArr[i + 3][y].getCheck() + _pArr[i + 4][y].getCheck();
    	}

    	if (res == 5) {
        	pX = i;
        	pY = y;
        	return 1;  // Người chơi X thắng
    	}
    	if (res == -5) {
        	pX = i;
        	pY = y;
        	return -1; // Người chơi O thắng
    	}
	}

	for (int i = yStart; i <= y; i++) {
    	if (i + 4 < _size) {
        	res = _pArr[x][i].getCheck() + _pArr[x][i + 1].getCheck() + _pArr[x][i + 2].getCheck() +
              	_pArr[x][i + 3].getCheck() + _pArr[x][i + 4].getCheck();
    	}

    	if (res == 5) {
        	pX = x;
        	pY = i;
        	return 1;
    	}
    	if (res == -5) {
        	pX = x;
        	pY = i;
        	return -1;
    	}
	}

	for (int i = xStart; i <= x; i++) {
    	for (int j = yStart; j <= y; j++) {
        	if (i - j == x - y && i + 4 < _size && j + 4 < _size) {
            	res = _pArr[i][j].getCheck() + _pArr[i + 1][j + 1].getCheck() + _pArr[i + 2][j + 2].getCheck() +
                	_pArr[i + 3][j + 3].getCheck() + _pArr[i + 4][j + 4].getCheck();
        	}

        	if (res == 5) {
            	pX = i;
            	pY = j;
            	return 1;
        	}
        	if (res == -5) {
            	pX = i;
            	pY = j;
            	return -1;
        	}
    	}
	}

	for (int i = xEnd; i >= x; i--) {
    	for (int j = yStart; j <= y; j++) {
        	if (i + j == x + y && j + 4 < _size && i - 4 >= 0) {
            	res = _pArr[i][j].getCheck() + _pArr[i - 1][j + 1].getCheck() + _pArr[i - 2][j + 2].getCheck() +
                	_pArr[i - 3][j + 3].getCheck() + _pArr[i - 4][j + 4].getCheck();
        	}
        
        	if (res == 5) {
            	pX = i;
            	pY = j;
            	return 1;
        	}
        	if (res == -5) {
            	pX = i;
            	pY = j;
            	return -1;
        	}
    	}
	}

	return 2; // Trả mặc định là tiep tuc
} 

//CPU
void _Board::CPU(int& x, int& y) {
    long long maxScore = 0;

    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _size; j++) {
            if (_pArr[i][j].getCheck() == 0) {
                long long attackScore = TC_CheoNguoc(i, j) + TC_CheoXuoi(i, j) + TC_Cot(i, j) + TC_Dong(i, j);
                long long defenseScore = PT_CheoNguoc(i, j) + PT_CheoXuoi(i, j) + PT_Cot(i, j) + PT_Dong(i, j);

                // Tăng trọng số cho các nước phòng thủ nguy hiểm
                if (defenseScore >= 10000) defenseScore *= 2; // Nếu đối thủ sắp thắng, chặn ngay

                // Cân nhắc tổng điểm
                long long totalScore = max(attackScore, defenseScore);

                // Ưu tiên nước đi
                if (totalScore > maxScore) {
                    maxScore = totalScore;
                    x = i;
                    y = j;
                }
            }
        }
    }
}

int _Board::checkBoardCPU(int& pX, int& pY, bool pTurn)
{
	if (pTurn)
	{
		for (int i = 0; i < _size; i++)
		{
			for (int j = 0; j < _size; j++)
			{
				if (_pArr[i][j].getX() == pX && _pArr[i][j].getY() == pY && _pArr[i][j].getCheck() == 0)
				{
					_pArr[i][j].setCheck(-1);// Nếu lượt hiện hành là true: c = -1
					return _pArr[i][j].getCheck();
				}
			}
		}
	}
	else
	{
 		int x = rand() % 4 + 4;
		int y = rand() % 4 + 4;

		CPU(x, y);

		_pArr[x][y].setCheck(1);
		pX = y * 4 + getLeft() + 2;
		pY = x * 2 + getTop() + 1;
		return _pArr[x][y].getCheck();
		
	}
	return 0;
}

//Attack

long long _Board::TC_Cot(int x, int y)
{
	long long total = 0;

	long long player = 0;
	long long computer = 0;

	for (int i = 1; (i < 6) && (x + i < _size); i++)
	{
		if (_pArr[x + i][y].getCheck() == 1)
		{
			computer++;
		}
		else if (_pArr[x + i][y].getCheck() == -1)
		{
			player++;
			break;
		}
		else
		{
			//Xu ly nhieu o trong;
			break;
		}
	}


	for (int i = 1; (i < 6) && (x - i >= 0); i++)
	{
		if (_pArr[x - i][y].getCheck() == 1)
		{
			computer++;
		}
		else if (_pArr[x - i][y].getCheck() == -1)
		{
			player++;
			break;
		}
		else
		{
			//Xu ly nhieu o trong;
			break;
		}
	}

	if (player == 2) return 0;

	total -= defense[player + 1];

	total += attack[computer];
	return total;
}

long long _Board::TC_Dong(int x, int y)
{
	long long total = 0;

	long long player = 0;
	long long computer = 0;

	for (int i = 1; i < 6 && y + i < _size; i++)
	{
		if (_pArr[x][y + i].getCheck() == 1)
		{
			computer++;
		}
		else if (_pArr[x][y + i].getCheck() == -1)
		{
			player++;
			break;
		}
		else
		{
			//Xu ly nhieu o trong;
			break;
		}
	}


	for (int i = 1; i < 6 && y - i >= 0; i++)
	{
		if (_pArr[x][y - i].getCheck() == 1)
		{
			computer++;
		}
		else if (_pArr[x][y - i].getCheck() == -1)
		{
			player++;
			break;
		}
		else
		{
			//Xu ly nhieu o trong;
			break;
		}
	}

	if (player == 2) return 0;

	total -= defense[player + 1];
	total += attack[computer];
	return total;
}

long long _Board::TC_CheoNguoc(int x, int y)
{
	long long total = 0;

	long long player = 0;
	long long computer = 0;

	for (int i = 1; i < 6 && y + i < _size && x - i >= 0; i++)
	{
		if (_pArr[x - i][y + i].getCheck() == 1)
		{
			computer++;
		}
		else if (_pArr[x - i][y + i].getCheck() == -1)
		{
			player++;
			break;
		}
		else
		{
			//Xu ly nhieu o trong;
			break;
		}
	}


	for (int i = 1; i < 6 && y - i >= 0 && x + i < _size; i++)
	{
		if (_pArr[x + i][y - i].getCheck() == 1)
		{
			computer++;
		}
		else if (_pArr[x + i][y - i].getCheck() == -1)
		{
			player++;
			break;
		}
		else
		{
			//Xu ly nhieu o trong;
			break;
		}
	}

	if (player == 2) return 0;

	total -= defense[player + 1];
	total += attack[computer];

	return total;
}

long long _Board::TC_CheoXuoi(int x, int y)
{
	long long total = 0;

	long long player = 0;
	long long computer = 0;

	for (int i = 1; i < 6 && y + i < _size && x + i < _size; i++)
	{
		if (_pArr[x + i][y + i].getCheck() == 1)
		{
			computer++;
		}
		else if (_pArr[x + i][y + i].getCheck() == -1)
		{
			player++;
			break;
		}
		else
		{
			//Xu ly nhieu o trong;
			break;
		}
	}


	for (int i = 1; i < 6 && y - i >= 0 && x - i >= 0; i++)
	{
		if (_pArr[x - i][y - i].getCheck() == 1)
		{
			computer++;
		}
		else if (_pArr[x - i][y - i].getCheck() == -1)
		{
			player++;
			break;
		}
		else
		{
			//Xu ly nhieu o trong;
			break;
		}
	}

	if (player == 2) return 0;

	total -= defense[player + 1];
	total += attack[computer];
	return total;
}

//Defense

long long _Board::PT_Cot(int x, int y)
{
	long long total = 0;

	long long player = 0;
	long long computer = 0;

	for (int i = 1; i < 6 && x + i < _size; i++)
	{
		if (_pArr[x + i][y].getCheck() == 1)
		{
			computer++;
			break;
		}
		else if (_pArr[x + i][y].getCheck() == -1)
		{
			player++;
		}
		else
		{
			//Xu ly nhieu o trong;
			break;
		}
	}


	for (int i = 1; i < 6 && x - i >= 0; i++)
	{
		if (_pArr[x - i][y].getCheck() == 1)
		{
			computer++;
			break;
		}
		else if (_pArr[x - i][y].getCheck() == -1)
		{
			player++;
		}
		else
		{
			//Xu ly nhieu o trong;
			break;
		}
	}

	if (computer == 2) return 0;

	total += defense[player];

	if (player > 0)
		total -= attack[computer] * 2;

	return total;
}

long long _Board::PT_Dong(int x, int y)
{
	long long total = 0;

	long long player = 0;
	long long computer = 0;

	for (int i = 1; i < 6 && y + i < _size; i++)
	{
		if (_pArr[x][y + i].getCheck() == 1)
		{
			computer++;
			break;
		}
		else if (_pArr[x][y + i].getCheck() == -1)
		{
			player++;
		}
		else
		{
			//Xu ly nhieu o trong;
			break;
		}
	}


	for (int i = 1; i < 6 && y - i >= 0; i++)
	{
		if (_pArr[x][y - i].getCheck() == 1)
		{
			computer++;
			break;
		}
		else if (_pArr[x][y - i].getCheck() == -1)
		{
			player++;
		}
		else
		{
			//Xu ly nhieu o trong;
			break;
		}
	}

	if (computer == 2) return 0;

	total += defense[player];

	if (player > 0)
		total -= attack[computer] * 2;
	return total;
}

long long _Board::PT_CheoNguoc(int x, int y)
{
	long long total = 0;

	long long player = 0;
	long long computer = 0;

	for (int i = 1; i < 6 && y + i < _size && x - i >= 0; i++)
	{
		if (_pArr[x - i][y + i].getCheck() == 1)
		{
			computer++;
			break;
		}
		else if (_pArr[x - i][y + i].getCheck() == -1)
		{
			player++;
		}
		else
		{
			//Xu ly nhieu o trong;
			break;
		}
	}


	for (int i = 1; i < 6 && y - i >= 0 && x + i < _size; i++)
	{
		if (_pArr[x + i][y - i].getCheck() == 1)
		{
			computer++;
			break;
		}
		else if (_pArr[x + i][y - i].getCheck() == -1)
		{
			player++;
		}
		else
		{
			//Xu ly nhieu o trong;
			break;
		}
	}

	if (computer == 2) return 0;
	
	total += defense[player];

	if (player > 0)
		total -= attack[computer] * 2;
	return total;
}

long long _Board::PT_CheoXuoi(int x, int y)
{
	long long total = 0;

	long long player = 0;
	long long computer = 0;

	for (int i = 1; i < 6 && y + i < _size && x + i < _size; i++)
	{
		if (_pArr[x + i][y + i].getCheck() == 1)
		{
			computer++;
			break;
		}
		else if (_pArr[x + i][y + i].getCheck() == -1)
		{
			player++;
		}
		else
		{
			//Xu ly nhieu o trong;
			break;
		}
	}


	for (int i = 1; i < 6 && y - i >= 0 && x - i >= 0; i++)
	{
		if (_pArr[x - i][y - i].getCheck() == 1)
		{
			computer++;
			break;
		}
		else if (_pArr[x - i][y - i].getCheck() == -1)
		{
			player++;
		}
		else
		{
			//Xu ly nhieu o trong;
			break;
		}
	}

	if (computer == 2) return 0;

	total += defense[player];

	if (player > 0)
		total -= attack[computer] * 2;
	return total;
}


//MiniMax ( Medium difficulty )
void _Board::Minimax(int& x, int& y)
{
		Move bestMove = findBestMoveMinimax();

		x = bestMove.col;
		y = bestMove.row;

}

// Alpha-Beta ( Hard difficulty )
void _Board::AlphaBeta(int& x, int& y) {
    
		Move bestMove = findBestMoveAlphaBeta();
    	
		x = bestMove.col;
    	y = bestMove.row;
}

bool _Board::isMovesLeft()
{
	for (int i = 0; i<_size; i++)
	for (int j = 0; j<_size; j++)
	if (_pArr[i][j].getCheck() == 0)
		return true;
	return false;
}

int _Board::evaluate()
{
	for (int i = 0; i < 200; i++)
	{
		for (int j = 0; j < 200; j++)
		{
			if (testBoard(i, j) != 0) return testBoard(i, j);
		}
	}
	return 0;
}

// Minimax algorithm
int _Board::minimax(int depth, bool isMax)
{
	int player = -1, opponent = 1;
	int score = evaluate(); // xu ly thang thua;

	// If Maximizer has won the game return his/her 
	// evaluated score 
	if (score == 1)
	{
		score = 10;
		return score;
	}

	// If Minimizer has won the game return his/her 
	// evaluated score 
	if (score == -1)
	{
		score = -10;
		return score;
	}

	// If there are no more moves and no winner then 
	// it is a tie 
	if (isMovesLeft() == false)
		return 0;

	// If this maximizer's move 
	if (isMax)
	{
		int best = -1000;

		// Traverse all cells 
		for (int i = 0; i<_size; i++)
		{
			for (int j = 0; j<_size; j++)
			{
				// Check if cell is empty 
				if (_pArr[i][j].getCheck() == 0)
				{
					// Make the move 
					_pArr[i][j].setCheck(player);

					// Call minimax recursively and choose 
					// the maximum value 
					best = max(best,
						minimax(depth + 1, !isMax));

					// Undo the move 
					_pArr[i][j].setCheck(0);
				}
			}
		}
		return best;
	}

	// If this minimizer's move 
	else
	{
		int best = 1000;

		// Traverse all cells 
		for (int i = 0; i<3; i++)
		{
			for (int j = 0; j<3; j++)
			{
				// Check if cell is empty 
				if (_pArr[i][j].getCheck() == 0)
				{
					// Make the move 
					_pArr[i][j].setCheck(opponent);

					// Call minimax recursively and choose 
					// the minimum value 
					best = min(best,
						minimax(depth + 1, !isMax));

					// Undo the move 
					_pArr[i][j].setCheck(0);
				}
			}
		}
		return best;
	}
}

Move _Board::findBestMoveMinimax()
{
	int player = -1, opponent = 1;
	int bestVal = -1000;
	Move bestMove;
	bestMove.row = -1;
	bestMove.col = -1;


	for (int i = 0; i<_size; i++)
	{
		for (int j = 0; j<_size; j++)
		{

			if (_pArr[i][j].getCheck() == 0)
			{
				// Make the move 
				_pArr[i][j].setCheck(player);

				// compute evaluation function for this 
				// move. 
				int moveVal = minimax(0, false);

				// Undo the move 
				_pArr[i][j].setCheck(0);

				// If the value of the current move is 
				// more than the best value, then update 
				// best/ 
				if (moveVal > bestVal)
				{
					bestMove.row = i;
					bestMove.col = j;
					bestVal = moveVal;
				}
			}
		}
	}
	return bestMove;
}


// Alpha-Beta Pruning Algorithm ( base on Minimax )
int _Board::alphabeta(int depth, bool isMax, int alpha, int beta) {
    int player = -1;  // Bot
    int opponent = 1; // Người chơi

    // Đánh giá trạng thái hiện tại
    int score = evaluate();

    // Điều kiện dừng: Trò chơi kết thúc hoặc đạt độ sâu tối đa
    if (score == 1) return 10 - depth; // Bot thắng
    if (score == -1) return depth - 10; // Người chơi thắng
    if (!isMovesLeft() || depth >= MAX_DEPTH) return 0; // Hòa hoặc hết nước đi

    if (isMax) {
        // Lượt của bot (Maximizer)
        int best = -1000;

        for (int i = 0; i < _size; i++) {
            for (int j = 0; j < _size; j++) {
                // Chỉ xét ô trống
                if (_pArr[i][j].getCheck() == 0) {
                    _pArr[i][j].setCheck(player); // Thử nước đi
                    int value = alphabeta(depth + 1, false, alpha, beta); // Đệ quy
                    best = max(best, value);
                    alpha = max(alpha, best); // Cập nhật alpha
                    _pArr[i][j].setCheck(0); // Hoàn tác nước đi

                    if (beta <= alpha) break; // Cắt tỉa
                }
            }
        }
        return best;
    } else {
        // Lượt của người chơi (Minimizer)
        int best = 1000;

        for (int i = 0; i < _size; i++) {
            for (int j = 0; j < _size; j++) {
                // Chỉ xét ô trống
                if (_pArr[i][j].getCheck() == 0) {
                    _pArr[i][j].setCheck(opponent); // Thử nước đi
                    int value = alphabeta(depth + 1, true, alpha, beta); // Đệ quy
                    best = min(best, value);
                    beta = min(beta, best); // Cập nhật beta
                    _pArr[i][j].setCheck(0); // Hoàn tác nước đi

                    if (beta <= alpha) break; // Cắt tỉa
                }
            }
        }
        return best;
    }
}


// Tìm nước đi tốt nhất sử dụng Alpha-Beta Pruning
Move _Board::findBestMoveAlphaBeta() {
    int player = -1;  // Bot
    int bestVal = -1000; // Giá trị tốt nhất ban đầu
    Move bestMove = {-1, -1}; // Nước đi tốt nhất khởi tạo mặc định

    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _size; j++) {
            // Chỉ xét ô trống
            if (_pArr[i][j].getCheck() == 0) {
                _pArr[i][j].setCheck(player); // Thử nước đi
                int moveVal = alphabeta(0, false, -1000, 1000); // Gọi Alpha-Beta
                _pArr[i][j].setCheck(0); // Hoàn tác nước đi

                // Cập nhật nước đi tốt nhất
                if (moveVal > bestVal) {
                    bestMove.row = i;
                    bestMove.col = j;
                    bestVal = moveVal;
                }
            }
        }
    }

    // Đảm bảo trả về nước đi hợp lệ
    if (bestMove.row == -1 || bestMove.col == -1) {
        std::cerr << "Error: Không tìm thấy nước đi hợp lệ!" << std::endl;
    }

    return bestMove;
}


