//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "Board.hpp"

bool Board::checkRow(int x, int y, int size) const
{
	BoardSquare c = _data[y][x];

	if (c == BoardSquare::black || c == BoardSquare::white)
	{
		while (--size)
		{
			++x;
			if (x >= BOARD_WIDTH || _data[y][x] != c)
				return false;
		}
		return (true);
	}
	return false;
}

bool Board::checkCol(int x, int y, int size) const
{
	BoardSquare c = _data[y][x];

	if (c == BoardSquare::black || c == BoardSquare::white)
	{
		while (--size)
		{
			++y;
			if (y >= BOARD_HEIGHT || _data[y][x] != c)
				return false;
		}
		return (true);
	}
	return false;
}

bool Board::checkDiagDown(int x, int y, int size) const
{
	BoardSquare c = _data[y][x];

	if (c == BoardSquare::black || c == BoardSquare::white)
	{
		while (--size)
		{
			++x;
			++y;
			if (x >= BOARD_WIDTH || y >= BOARD_HEIGHT || _data[y][x] != c)
				return false;
		}
		return (true);
	}
	return false;
}

bool Board::checkDiagUp(int x, int y, int size) const
{
	BoardSquare c = _data[y][x];

	if (c == BoardSquare::black || c == BoardSquare::white)
	{
		while (--size)
		{
			++x;
			--y;
			if (x >= BOARD_WIDTH || y < 0 || _data[y][x] != c)
				return false;
		}
		return (true);
	}
	return false;
}

bool Board::isAlignedStone(int size) const
{
	for (int y = 0; y < BOARD_HEIGHT; ++y)
	{
		for (int x = 0; x < BOARD_WIDTH; ++x)
		{
			if (checkRow(x, y, size)
					|| checkCol(x, y, size)
					|| checkDiagDown(x, y, size)
					|| checkDiagUp(x, y, size))
				return (true);
		}
	}
	return false;
}

bool Board::isTerminal()
{
	if (isAlignedStone(5))
		return true;
	if (_capturedWhites > 10 || _capturedBlacks > 10)
		return true;
	return  false;
}

std::vector<Board*> Board::getChildren(PlayerColor player)
{
	auto children = std::vector<Board*>(BOARD_HEIGHT * BOARD_WIDTH);
	int index = 0;

	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; y++)
		{
			BoardSquare square = (_data[y][x]);
			if (square == BoardSquare::empty)
			{
				children[index] = new Board(
						*this,
						BoardPos(x, y),
						player);
				index++;
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
			if (playCaptureDir(x, y, dirX, dirY, c)) {
				_data[y + dirY*1][x + dirX*1] = c;
				_data[y + dirY*2][x + dirX*2] = c;
				return (true);
			}
	return false;
}

Board::Board(): _data(), _capturedWhites(), _capturedBlacks(), _move() { }

Board::Board(Board& board):Board()
{
	*this = board;
}

Board::Board(Board& board, BoardPos move, PlayerColor player):Board(board)
{
	if (player == PlayerColor::whitePlayer)
		_data[move.y][move.x] = BoardSquare::white;
	else
		_data[move.y][move.x] = BoardSquare::black;

	if (playCapture(move.x, move.y))
	{
		if (player == PlayerColor::whitePlayer)
			_capturedBlacks += 2;
		else
			_capturedWhites += 2;
	}
}

BoardData* Board::getData() { return &_data; }
BoardPos Board::getMove() { return _move; }

BoardSquare	Board::getCase(int x, int y) const { return (_data[y][x]); }