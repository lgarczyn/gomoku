//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "Board.hpp"
#include "Game.hpp"
#include "MoveScore.hpp"
#include <algorithm>	// min max
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

bool Board::isAlignedStoneDir(int x, int y, int dirX, int dirY, BoardSquare good, int size) const
{
	for (int i = 0 ; i < size ; ++i) {
		if (x + i * dirX < 0 || x + i * dirX >= BOARD_WIDTH
			|| y + i * dirY < 0 || y + i * dirY >= BOARD_HEIGHT)
			return (false);
		if (_data[y + dirY*i][x + dirX*i] != good)
			return (false);
	}
	return (true);
}

bool Board::isAlignedStone(int size) const
{
	for (int y = 0 ; y < BOARD_HEIGHT; ++y)
		for (int x = 0 ; x < BOARD_WIDTH; ++x)
			for (int dirX = -1 ; dirX <= 1; ++dirX)
				for (int dirY = -1 ; dirY <= 1; ++dirY)
					if (dirX || dirY) {
						auto c = _data[y][x];
						if (c == BoardSquare::white || c == BoardSquare::black)
						if (isAlignedStoneDir(x, y, dirX, dirY, c, size)) {
							return (true);
						}
					}
	return false;
}

int 		Board::getCapturedBlack() const
{
	return (_capturedBlacks);
}

int 		Board::getCapturedWhite() const
{
	return (_capturedWhites);
}

VictoryState  Board::isTerminal(bool considerCapture)
{
	if (isAlignedStone(5))
		return aligned;
	if (considerCapture)
	{
		if (_capturedWhites > 10)
			return whitesCaptured;
		if (_capturedBlacks > 10)
			return blacksCaptured;
	}
	return  novictory;
}

std::vector<ChildBoard> Board::getChildren(PlayerColor player, bool capture, int count = -1)
{
	fillPriority();

	auto childrenPos = std::vector<MoveScore>();

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			int score = _priority[y][x];
			childrenPos.push_back(MoveScore(score, BoardPos(x, y)));
		}
	}

	shuffle (childrenPos.begin(), childrenPos.end(), std::default_random_engine(std::random_device{}()));

	struct Sorter
	{
		Sorter(){};
		bool operator () (const MoveScore & a, const MoveScore & b)
		{
			return a.score > b.score;
		}
	};

	sort(childrenPos.begin(), childrenPos.end(), Sorter());

	auto children = std::vector<ChildBoard>();
	for (auto move:childrenPos)
	{
		if (children.size() >= count && count >= 0)
			break;
		children.push_back(ChildBoard(
				new Board(*this, move.pos, player, capture),
				move.pos));
	}
	return (children);
}


bool Board::playCaptureDir(int x, int y, int dirX, int dirY, BoardSquare good) {

	BoardSquare bad = (good == BoardSquare::white ? BoardSquare::black : BoardSquare::white);

	if (x + 3*dirX < 0 || x + 3*dirX >= BOARD_WIDTH
		|| y + 3*dirY < 0 || y + 3*dirY >= BOARD_HEIGHT)
		return (false);
	return (_data[y][x] == good
			&& _data[y + dirY*1][x + dirX*1] == bad
			&& _data[y + dirY*2][x + dirX*2] == bad
			&& _data[y + dirY*3][x + dirX*3] == good);
}

bool Board::playCapture(int x, int y) {
	BoardSquare c = _data[y][x];

	for (int dirX = -1 ; dirX <= 1; ++dirX)
		for (int dirY = -1 ; dirY <= 1; ++dirY)
			if (dirX || dirY) {
				if (playCaptureDir(x, y, dirX, dirY, c)) {
					_data[y + dirY * 1][x + dirX * 1] = BoardSquare::empty;
					_data[y + dirY * 2][x + dirX * 2] = BoardSquare::empty;
					return (true);
				}
			}
	return false;
}

//TODO check algo is accurate
bool Board::checkFreeThree(int x, int y, int dirX, int dirY, BoardSquare enemy)
{
	int ix = x + 4 * -dirX;
	int iy = y + 4 * -dirY;
	int mx = x + 4 * dirX;
	int my = y + 4 * dirY;

	//TODO find better way (seriously tough)
	while (ix < 0 || iy < 0 || ix >= BOARD_WIDTH || iy >= BOARD_HEIGHT)
		ix += dirX, iy += dirY;
	while (mx < 0 || my < 0 || mx >= BOARD_WIDTH || my >= BOARD_HEIGHT)
		mx -= dirX, my -= dirY;

	BoardSquare buffer[6];// = {BoardSquare::empty };//TODO remove init
	int bufferIndex = 0;
	bool didLoop = false;

	//TODO find better way
	while (ix * dirX <= mx * dirX && iy * dirY <= my * dirY)
	{
		BoardSquare tmp = _data[iy][ix];
		buffer[bufferIndex] = tmp;

		++bufferIndex;

		if (bufferIndex == 6)
		{
			bufferIndex = 0;
			didLoop = true;
		}

		if (didLoop)
		{
			if (tmp == empty && buffer[bufferIndex] == empty)
			{
				int emptyCount = 0;
				bool foundEnemy = false;
				for (BoardSquare square:buffer)
				{
					if (square == enemy)
					{
						foundEnemy = true;
						break;
					}
					if (square == BoardSquare::empty)
						emptyCount++;
				}
				if (!foundEnemy && emptyCount == 4)
					return true;
			}
		}
		ix += dirX, iy += dirY;
	}
	return false;
}

//TODO BUG CURRENTLY TABOO POS ARE ERASED BY PRIORITY
void Board::fillTaboo(bool limitBlack, bool doubleThree, PlayerColor player)
{
	BoardSquare enemy = (player == blackPlayer)? white : black;
	if (limitBlack)
	{
		if (_turnNum == 0)
			for (int y = 0; y < BOARD_HEIGHT; y++)
			{
				for (int x = 0; x < BOARD_WIDTH; x++)
				{
					if (x != 9 || y != 9)
						if (_data[y][x] == BoardSquare::empty)
							_priority[y][x] = -1;
				}
			}
		else if (_turnNum == 2)
			for (int y = 4; y < 15; y++)
			{
				for (int x = 4; x < 15; x++)
				{
					_priority[y][x] = -1;
				}
			}
	}
	if (doubleThree)
	{//TODO remove line from all sides (???)
		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			for (int x = 0; x < BOARD_WIDTH; x++)
			{
				if (_data[y][x] == BoardSquare::empty)
				{
					int count = 0;
					if (checkFreeThree(x, y, 1, 0, enemy)) count++;
					if (checkFreeThree(x, y, 1, 1, enemy)) count++;
					if (count >= 2) {_priority[y][x] = -1; continue;}
					if (checkFreeThree(x, y, 0, 1, enemy)) count++;
					if (count >= 2) {_priority[y][x] = -1; continue;}
					if (checkFreeThree(x, y, -1, 1, enemy)) count++;
					if (count >= 2) {_priority[y][x] = -1; continue;}
				}
			}
		}
	}
}

inline int clamp(int value, int min, int max)
{
	if (value <= min)
		return min;
	if (value >= max)
		return max;
	return value;
}

//inline //TODO put back
void Board::fillPriorityDir(int x, int y, int dirX, int dirY, BoardSquare ally)
{
	const int maxX = clamp(x + 5 * dirX, -1, BOARD_WIDTH);
	const int maxY = clamp(y + 5 * dirY, -1, BOARD_HEIGHT);

	int value = 1;

	x += dirX, y+= dirY;
	//wait for both iterator to either be static or have reached their goal
	while ((!dirX || x != maxX) && (!dirY || y != maxY))
	{
		BoardSquare square = _data[y][x];
		if (square == empty && _priority[y][x] >= 0)
		{
			_priority[y][x] += value;
		}
		else if (square == ally)
		{
			value <<= 1;
		}
		else
		{
			break;
		}
		x += dirX, y+= dirY;
	}
};

void Board::fillPriority()
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			BoardSquare ally = _data[y][x];
			if (ally != BoardSquare::empty)
			{
				fillPriorityDir(x, y, -1, -1, ally);
				fillPriorityDir(x, y, -1, 0, ally);
				fillPriorityDir(x, y, -1, 1, ally);
				fillPriorityDir(x, y, 0, -1, ally);
				//fillPriorityDir(x, y, 0, 0, ally);
				fillPriorityDir(x, y, 0, 1, ally);
				fillPriorityDir(x, y, 1, -1, ally);
				fillPriorityDir(x, y, 1, 0, ally);
				fillPriorityDir(x, y, 1, 1, ally);
			}
		}
	}
}

bool Board::isPosLegal(int x, int y, bool limitBlack, bool doubleThree, PlayerColor player)
{
	BoardSquare enemy = (player == blackPlayer)? white : black;

	if (limitBlack)
	{
		if (_turnNum == 0)
		{
			if (y != 9 && x != 9)
				return false;
			else
				return true;
		}
		else if (_turnNum == 2)
		{
			if (y >= 4 && y < 15 && x >= 4 && x < 15)
				return false;
			else
				return true;
		}
	}
	int count = 0;
	if (checkFreeThree(x, y, 1, 0, enemy)) count++;
	if (checkFreeThree(x, y, 1, 1, enemy)) count++;
	if (count >= 2) { return false; }
	if (checkFreeThree(x, y, 0, 1, enemy)) count++;
	if (count == 0) { return true; }
	if (count >= 2) { return false; }
	if (checkFreeThree(x, y, -1, 1, enemy)) count++;
	if (count >= 2) { return false; }
	return true;
}

Board::Board(): _data(), _priority(), _capturedWhites(), _capturedBlacks(), _turnNum(), hasScore() { }

Board::Board(const Board& board):Board()
{
	*this = board;
	bzero(_priority, sizeof(_priority));//TODO reuse score from previous
}

Board::Board(const Board& board, BoardPos move, PlayerColor player, bool capture) : Board(board)
{
	if (player == PlayerColor::whitePlayer)
		_data[move.y][move.x] = BoardSquare::white;
	else
		_data[move.y][move.x] = BoardSquare::black;

	if (capture)
	{
		if (playCapture(move.x, move.y))
		{
			if (player == PlayerColor::whitePlayer)
				_capturedBlacks += 2;
			else
				_capturedWhites += 2;
		}
	}
	_turnNum = board._turnNum + 1;
}

BoardData* Board::getData() { return &_data; }
BoardSquare	Board::getCase(BoardPos pos) const { return (_data[pos.y][pos.x]); }
BoardSquare&	Board::getCase(BoardPos pos) { return (_data[pos.y][pos.x]); }
BoardSquare		Board::getCase(int x, int y) const {return (_data[y][x]);};
BoardSquare&	Board::getCase(int x, int y) {return (_data[y][x]);};
int 			Board::getPriority(int x, int y) const {return _priority[y][x];}
int 			Board::getPriority(BoardPos pos) const {return _priority[pos.y][pos.x];}

BoardPos		Board::getBestPriority() const
{
	MoveScore best = MoveScore(-1, BoardPos());

	for (BoardPos current; current != BoardPos::boardEnd; ++current)
	{
		int p = getPriority(current);
		if (p > best.score)
		{
			best = MoveScore(p, current);
		}
	}
	return (best.pos);
}