//
// Created by Louis GARCZYNSKI on 3/30/16.
//

class Board;

#pragma once

#include "BoardSquare.hpp"

#include <vector>

#include "PlayerColor.hpp"
#include "BoardPos.hpp"
#include "Game.hpp"

#define BOARD_WIDTH 19
#define BOARD_HEIGHT 19

using BoardData = BoardSquare[BOARD_WIDTH][BOARD_HEIGHT];

class Board
{
public:

	bool isTerminal();
	std::vector<Board*> getChildren(PlayerColor player);
	Board();
	Board(Board& board);
	Board(Board& board, BoardPos move, PlayerColor player);

	BoardData* getData();
	BoardSquare	getCase(int x, int y) const;
	BoardPos getMove() const;

private:
	BoardData	_data;
	BoardPos	_move;
	int			_capturedWhites;
	int			_capturedBlacks;

	bool 		playCapture(int x, int y);
	bool 		playCaptureDir(int x, int y, int dirX, int dirY, BoardSquare type);
	bool		checkRow(int x, int y, int size) const;
	bool		checkCol(int x, int y, int size) const;
	bool		checkDiagUp(int x, int y, int size) const;
	bool		checkDiagDown(int x, int y, int size) const;
	bool		isAlignedStone(int size) const;
};


