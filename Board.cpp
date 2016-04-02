//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "Board.hpp"

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

VictoryState Board::isTerminal()
{
	if (isAlignedStone(5))
		return aligned;
	if (_capturedWhites > 10)
		return whitesCaptured;
	if (_capturedBlacks > 10)
		return blacksCaptured;
	return novictory;
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

std::vector<ChildBoard> Board::getChildren(PlayerColor player) const
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
							new Board(*this, BoardPos(x, y), player),
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

Board::Board(): _data(), _capturedWhites(), _capturedBlacks() { }

Board::Board(const Board& board):Board()
{
	*this = board;
}

Board::Board(const Board& board, BoardPos move, PlayerColor player) : Board(board)
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
BoardSquare	Board::getCase(int x, int y) const { return (_data[y][x]); }
BoardSquare	Board::getCase(BoardPos pos) const { return (_data[pos.y][pos.x]); }