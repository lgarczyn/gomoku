//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "Board.hpp"
#include "Game.hpp"

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

bool Board::isTerminal(bool considerCapture)
{
	if (isAlignedStone(5))
		return true;
	if (considerCapture && (_capturedWhites > 10 || _capturedBlacks > 10))
		return true;
	return  false;
}

bool Board::isPosInterest(int x, int y, PlayerColor player) const
{
	int half_zone_size = 4;

	for (int dirY = -half_zone_size ; dirY <= half_zone_size; ++dirY)
		for (int dirX = -half_zone_size ; dirX <= half_zone_size; ++dirX) {
			if (x + dirX < 0 || x + dirX >= BOARD_WIDTH
				|| y + dirY < 0 || y + dirY >= BOARD_HEIGHT)
				continue ;
			if (_data[y + dirY][x + dirX] != BoardSquare::empty)
				return (true);
		}
	return (false);
}

std::vector<ChildBoard> Board::getChildren(PlayerColor player, bool capture) const
{
	auto children = std::vector<ChildBoard>(BOARD_HEIGHT * BOARD_WIDTH);
	int index = 0;

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			BoardSquare square = (_data[y][x]);

			if (square == BoardSquare::empty)
			{
				if (this->isPosInterest(x, y, player)) {
					children[index] = std::make_tuple(
							new Board(
									*this,
									BoardPos(x, y),
									player,
									capture
							),
							BoardPos(x, y)
					);
					index++;
				}
			}
		}
	}
	children.resize(index);
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

int inline clamp(int value, int min, int max)
{
	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}

bool Board::checkFreeThree(int x, int y, int dirX, int dirY, BoardSquare enemy)
{
	int ix = clamp(x + 4 * -dirX, 0, BOARD_WIDTH - 1);
	int iy = clamp(y + 4 * -dirY, 0, BOARD_HEIGHT - 1);
	int mx = clamp(x + 4 * dirX, 0, BOARD_WIDTH - 1);
	int my = clamp(y + 4 * dirY, 0, BOARD_HEIGHT - 1);

	BoardSquare buffer[6] = {BoardSquare::empty };//TODO remove init
	int bufferIndex = 0;
	bool didLoop = false;

	while (ix <= mx && iy <= my)
	{
		if (bufferIndex == 5)
			didLoop = true;
		if (bufferIndex == 6)
			bufferIndex = 0;

		BoardSquare tmp = _data[iy][ix];
		buffer[bufferIndex] = (tmp == taboo) ? empty : tmp;

		if (didLoop)
		{
			if (tmp == empty && buffer[(bufferIndex + 1) % 6] == empty)
				//TODO check if really need to be empty or just non-enemy
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

		bufferIndex++;
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
							_data[y][x] = BoardSquare::taboo;
				}
			}
		else if (_turnNum == 2)
			for (int y = 4; y < 15; y++)
			{
				for (int x = 4; x < 15; x++)
				{
					if (_data[y][x] == BoardSquare::empty)
						_data[y][x] = BoardSquare::taboo;
				}
			}
	}
	if (doubleThree)
	{//TODO remove line from all sides
		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			for (int x = 0; x < BOARD_WIDTH; x++)
			{
				if (_data[y][x] == BoardSquare::empty)
				{
					int count = 0;
					if (checkFreeThree(x, y, 1, 0, enemy)) count++;
					if (checkFreeThree(x, y, 1, 1, enemy)) count++;
					if (count >= 2) {_data[y][x] = BoardSquare::taboo; continue;}
					if (checkFreeThree(x, y, 0, 1, enemy)) count++;
					if (count >= 2) {_data[y][x] = BoardSquare::taboo; continue;}
					if (checkFreeThree(x, y, -1, 1, enemy)) count++;
					if (count >= 2) {_data[y][x] = BoardSquare::taboo; continue;}
				}
			}
		}
	}

}

Board::Board(): _data(), _capturedWhites(), _capturedBlacks(), _turnNum() { }

Board::Board(const Board& board):Board()
{
	*this = board;
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			if (_data[y][x] == taboo)
				_data[y][x] = empty;
		}
	}
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
BoardSquare	Board::getCase(int x, int y) const { return (_data[y][x]); }
BoardSquare	Board::getCase(BoardPos pos) const { return (_data[pos.y][pos.x]); }