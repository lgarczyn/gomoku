//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#pragma once

#include <vector>

#include "Constants.hpp"
#include "BoardPos.hpp"
#include "PlayerColor.hpp"
#include "BoardSquare.hpp"

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
	BoardSquare	getCase(BoardPos pos) const;
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


