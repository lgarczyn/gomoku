//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#pragma once

#include "BoardSquare.hpp"

#include <vector>

#include "BoardPos.hpp"

#define BOARD_WIDTH 19
#define BOARD_HEIGHT 19

using BoardData = BoardSquare[BOARD_WIDTH][BOARD_HEIGHT];

class Board
{
public:

	bool isTerminal();
	std::vector<Board*> getChildren();
	Board();
	Board(Board& board);

	bool		checkRow(int x, int y, int size) const;
	bool		checkCol(int x, int y, int size) const;
	bool		checkDiagUp(int x, int y, int size) const;
	bool		checkDiagDown(int x, int y, int size) const;
	bool		isAlignedStone(int size) const;
	BoardSquare	getCase(int x, int y) const;

	BoardData* getData();
	BoardPos getMove();
	int getCaptured();

private:
	BoardData _data;
	BoardPos _move;
	int _captured;
};


