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
	return isAlignedStone(5);
}

std::vector<Board*> Board::getChildren()
{
	return std::vector<Board*>(0);//TODO implement
}

Board::Board(): _data(), _captured(), _move()
{

}

Board::Board(Board& board):_captured(), _move()
{
	memcpy(&_data, &board._data, sizeof(_data));
}

BoardData* Board::getData() { return &_data; }
BoardPos Board::getMove() { return _move; }
int Board::getCaptured() { return _captured; }

BoardSquare	Board::getCase(int x, int y) const { return (_data[y][x]); }