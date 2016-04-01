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
	BoardSquare	getCase(BoardPos pos) const;
	BoardPos getMove() const;

	bool 		playCapture(int x, int y);
	bool 		playCaptureDir(int x, int y, int dirX, int dirY, BoardSquare type);
	bool 		isAlignedStoneDir(int x, int y, int dirX, int dirY, BoardSquare good, int size) const;
	bool		isAlignedStone(int size) const;
private:
	BoardData	_data;
	BoardPos	_move;
	int			_capturedWhites;
	int			_capturedBlacks;

};


