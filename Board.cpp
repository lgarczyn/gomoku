//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "Board.hpp"
#include <algorithm>	// min max
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

/*bool Board::isAlignedStoneDir(int x, int y, int dirX, int dirY, BoardSquare good, int size) const
{
	for (int i = 1 ; i < size ; ++i) {
		x += dirX;
		y += dirY;

		if (x  < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT)
			return (false);
		if (_data[y][x] != good)
			return (false);
	}
	return (true);
}

bool Board::isAlignedStonePos(int x, int y, int size) const
{
	BoardSquare c = _data[y][x];
	if (c == empty) return false;
	if (isAlignedStoneDir(x, y, -1, -1, c, size)) return (true);
	if (isAlignedStoneDir(x, y, -1, 0, c, size)) return (true);
	if (isAlignedStoneDir(x, y, -1, 1, c, size)) return (true);
	if (isAlignedStoneDir(x, y, 0, -1, c, size)) return (true);
	//if (isAlignedStoneDir(x, y, 0, 0, c, size)) return (true);
	if (isAlignedStoneDir(x, y, 0, 1, c, size)) return (true);
	if (isAlignedStoneDir(x, y, 1, -1, c, size)) return (true);
	if (isAlignedStoneDir(x, y, 1, 0, c, size)) return (true);
	if (isAlignedStoneDir(x, y, 1, 1, c, size)) return (true);
	return false;





}

bool Board::isAlignedStone(int size) const
{
	for (int y = 0 ; y < BOARD_HEIGHT; ++y)
		for (int x = 0 ; x < BOARD_WIDTH; ++x)
			if (isAlignedStonePos(x, y, size))
				return true;
	return false;
}*/



inline bool Board::isAlignedStoneDir(int x, int y, int dirX, int dirY, BoardSquare color, int size) const
{
	int ix = x + (size - 1) * -dirX;
	int iy = y + (size - 1) * -dirY;
	int mx = x + size * dirX;
	int my = y + size * dirY;

	//TODO find better way (seriously tough)
	while (ix < 0 || iy < 0 || ix >= BOARD_WIDTH || iy >= BOARD_HEIGHT)
		ix += dirX, iy += dirY;
	while (mx < -1 || my < -1 || mx > BOARD_WIDTH || my > BOARD_HEIGHT)
		mx -= dirX, my -= dirY;

	int count = 0;

	while ((dirX == 0 || ix != mx) && (dirY == 0 || iy != my))
	{
		if (_data[iy][ix] != color)
		{
			count = 0;
		}
		else if (++count == size)
		{
			return true;
		}
		ix += dirX, iy += dirY;
	}
	return false;
}

bool Board::isAlignedStonePos(int x, int y, int size) const
{
	BoardSquare c = _data[y][x];
	if (c == empty) return false;

	if (isAlignedStoneDir(x, y, 1, 0, c, size)) return true;
	if (isAlignedStoneDir(x, y, 1, 1, c, size)) return true;
	if (isAlignedStoneDir(x, y, 0, 1, c, size)) return true;
	if (isAlignedStoneDir(x, y, -1, 1, c, size)) return true;
	return false;
}

bool Board::isAlignedStone(int size) const
{
	for (int y = 0 ; y < BOARD_HEIGHT; ++y)
		for (int x = 0 ; x < BOARD_WIDTH; ++x)
			if (_data[y][x] != empty)
				if (isAlignedStonePos(x, y, size))
					return true;
	return false;
}


VictoryState  Board::isTerminal(bool considerCapture)
{
	if (isAlignedStone(5))
		return aligned;
	if (considerCapture)
	{
		if (_capturedWhites >= captureVictoryPoints)
			return whitesCaptured;
		if (_capturedBlacks >= captureVictoryPoints)
			return blacksCaptured;
	}
	return  novictory;
}

VictoryState  Board::isTerminal(BoardPos pos, bool considerCapture)
{
	if (isAlignedStonePos(pos.x, pos.y, 5))
		return aligned;
	if (considerCapture)
	{
		if (_capturedWhites >= captureVictoryPoints)
			return whitesCaptured;
		if (_capturedBlacks >= captureVictoryPoints)
			return blacksCaptured;
	}
	if (_turnNum - _capturedBlacks - _capturedWhites == BOARD_HEIGHT * BOARD_WIDTH)
	{
		return staleMate;
	}
	return  novictory;
}

std::vector<ChildBoard> Board::getChildren(PlayerColor player, bool capture, size_t count = -1)
{
	fillPriority(player);

	//EITHER
	//set finished boards priority to higher
	//

	auto childrenPos = std::vector<MoveScore>();

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			if (_data[y][x] == empty)
			{
				int score = _priority[y][x];
				if (score)
				{
					childrenPos.push_back(MoveScore(score, BoardPos(x, y)));
				}
			}
		}
	}

	//shuffle (childrenPos.begin(), childrenPos.end(), std::default_random_engine(std::random_device{}()));
	//only shuffle is first?
	//TODO use boost::qsort
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
		if (children.size() >= count)
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

int Board::playCapture(int x, int y) {
	BoardSquare c = _data[y][x];

	int capCount = 0;
	for (int dirX = -1 ; dirX <= 1; ++dirX)
		for (int dirY = -1 ; dirY <= 1; ++dirY)
			if (dirX || dirY) {
				if (playCaptureDir(x, y, dirX, dirY, c)) {
					_data[y + dirY * 1][x + dirX * 1] = BoardSquare::empty;
					_data[y + dirY * 2][x + dirX * 2] = BoardSquare::empty;
					++capCount;
				}
			}
	return capCount;
}

bool Board::checkFreeThree(int x, int y, int dirX, int dirY, BoardSquare enemy)
{
	int ix = x + 4 * -dirX;
	int iy = y + 4 * -dirY;
	int mx = x + 5 * dirX;
	int my = y + 5 * dirY;

	//TODO find better way (seriously tough)
	while (ix < 0 || iy < 0 || ix >= BOARD_WIDTH || iy >= BOARD_HEIGHT)
		ix += dirX, iy += dirY;
	while (mx < -1 || my < -1 || mx > BOARD_WIDTH || my > BOARD_HEIGHT)
		mx -= dirX, my -= dirY;

	BoardSquare buffer[6];
	int bufferIndex = 0;
	bool didLoop = false;

	while ((!dirX || ix != mx) && (!dirY || iy != my))
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
	{
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


inline void Board::fillPriorityDir(int x, int y, int dirX, int dirY, BoardSquare color)//, int bonus)
{
	const int maxX = CLAMP(x + 5 * dirX, -1, BOARD_WIDTH);
	const int maxY = CLAMP(y + 5 * dirY, -1, BOARD_HEIGHT);

	int value = 1;// + bonus;
	int count = 0;

	x += dirX, y+= dirY;
	//wait for both iterator to either be static or have reached their goal
	while ((!dirX || x != maxX) && (!dirY || y != maxY))
	{
		BoardSquare square = _data[y][x];
		if (square == color)
		{
			value <<= 3;
		}
		else if (square == empty)
		{
			if (_priority[y][x] >= 0)
			{
				_priority[y][x] += value;
			}
		}
		else
		{
			count++;
			break;
		}
		x += dirX, y+= dirY;
		count++;
	}
	value >>= 2;
	while (count > 0)
	{
		count--;
		x -= dirX, y -= dirY;

		BoardSquare square = _data[y][x];
		if (square == empty && _priority[y][x] >= 0)
		{
			_priority[y][x] += value;
		}
	}
};

void Board::fillCapturePriorityDir(int x, int y, int dirX, int dirY, BoardSquare color)
{
	int endX = x + dirX * 3;
	int endY = y + dirY * 3;

	if (endX >= 0 && endX < BOARD_WIDTH && endY >= 0 && endY < BOARD_HEIGHT)
		if (_data[y + dirY * 1][x + dirX * 1] == color &&
			_data[y + dirY * 2][x + dirX * 2] == color &&
			_data[endY][endX] == empty)
		{
			_priority[endY][endX] += capturePriority;
		}
};

void Board::fillPriority(PlayerColor player)
{
	BoardSquare ally = (player == blackPlayer)? black : white;
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			BoardSquare color = _data[y][x];
			int bonus = (ally != color);
			if (color != BoardSquare::empty)
			{
				BoardSquare enemyColor = (color == white) ? black : white;

				fillPriorityDir(x, y, -1, -1, color);//, bonus);
				fillPriorityDir(x, y, -1, 0, color);//, bonus);
				fillPriorityDir(x, y, -1, 1, color);//, bonus);
				fillPriorityDir(x, y, 0, -1, color);//, bonus);
				//fillPriorityDir(x, y, 0, 0, color);//, bonus);
				fillPriorityDir(x, y, 0, 1, color);//, bonus);
				fillPriorityDir(x, y, 1, -1, color);//, bonus);
				fillPriorityDir(x, y, 1, 0, color);//, bonus);
				fillPriorityDir(x, y, 1, 1, color);//, bonus);

				fillCapturePriorityDir(x, y, -1, -1, enemyColor);
				fillCapturePriorityDir(x, y, -1, 0, enemyColor);
				fillCapturePriorityDir(x, y, -1, 1, enemyColor);
				fillCapturePriorityDir(x, y, 0, -1, enemyColor);
				//fillCapturePriorityDir(x, y, 0, 0, enemyColor);
				fillCapturePriorityDir(x, y, 0, 1, enemyColor);
				fillCapturePriorityDir(x, y, 1, -1, enemyColor);
				fillCapturePriorityDir(x, y, 1, 0, enemyColor);
				fillCapturePriorityDir(x, y, 1, 1, enemyColor);
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
	if (doubleThree)
	{
		int count = 0;
		if (checkFreeThree(x, y, 1, 0, enemy)) count++;
		if (checkFreeThree(x, y, 1, 1, enemy)) count++;
		if (count >= 2) { return false; }
		if (checkFreeThree(x, y, 0, 1, enemy)) count++;
		if (count == 0) { return true; }
		if (count >= 2) { return false; }
		if (checkFreeThree(x, y, -1, 1, enemy)) count++;
		if (count >= 2) { return false; }
	}
	return true;
}

Board::Board(): _data(), _priority(), _capturedWhites(), _capturedBlacks(), _turnNum()
{
	_priority[9][9] = 1;
}

Board::Board(const Board& board)
{
	*this = board;
	bzero(_priority, sizeof(_priority));
}

Board::Board(const Board& board, BoardPos move, PlayerColor player, bool capture) : Board(board)
{
	if (player == PlayerColor::whitePlayer)
		_data[move.y][move.x] = BoardSquare::white;
	else
		_data[move.y][move.x] = BoardSquare::black;

	if (capture)
	{
		int captures = playCapture(move.x, move.y);
		if (player == PlayerColor::whitePlayer)
			_capturedBlacks += 2 * captures;
		else
			_capturedWhites += 2 * captures;
	}
	_turnNum = board._turnNum + 1;
}

Board::~Board() { }

MoveScore		Board::getBestPriority() const
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
	return (best);
}