//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#pragma once

#include "BoardSquare.hpp"

#include <vector>

#include "BoardPos.hpp"

using BoardData = BoardSquare[19][19];

class Board
{
public:

	bool isTerminal();
	std::vector<Board*> getChildren();
	Board();
	Board(Board& board);

	BoardData* getData();
	BoardPos getMove();
	int getCaptured();

private:
	BoardData _data;
	BoardPos _move;
	int _captured;

};


