//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "Board.hpp"


bool Board::isAlignedStoneDir(int x, int y, int dirX, int dirY, BoardSquare good, int size) const
{

	for (int i = 0 ; i < size ; ++i) {
		if (x + 3 * dirX < 0 || x + 3 * dirX >= BOARD_WIDTH
			|| y + 3 * dirY < 0 || y + 3 * dirY >= BOARD_HEIGHT)
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
					if (isAlignedStoneDir(x, y, dirX, dirY, _data[y][x], size)) {
						return (true);
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
		for (int x = 0; x < BOARD_WIDTH; x++)
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
				_data[y + dirY*1][x + dirX*1] = BoardSquare::empty;
				_data[y + dirY*2][x + dirX*2] = BoardSquare::empty;
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
	_move = move;
}

BoardData* Board::getData() { return &_data; }
BoardPos Board::getMove() const { return _move; }
BoardSquare	Board::getCase(int x, int y) const { return (_data[y][x]); }
BoardSquare	Board::getCase(BoardPos pos) const { return (_data[pos.y][pos.x]); }