//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "Board.hpp"

bool Board::isTerminal()
{
	return false;//TODO implement
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