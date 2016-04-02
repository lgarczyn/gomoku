//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#pragma once

#include <vector>
#include <tuple>

#include "Constants.hpp"
#include "BoardPos.hpp"
#include "PlayerColor.hpp"
#include "BoardSquare.hpp"
#include "VictoryState.hpp"

class Board;

using BoardData = BoardSquare[BOARD_WIDTH][BOARD_HEIGHT];
using ChildBoard = std::tuple<Board*, BoardPos>;

class Board
{
public:
	VictoryState isTerminal();
	bool isPosInterest(int x, int y, PlayerColor player) const;
	std::vector<ChildBoard> getChildren(PlayerColor player) const;
	Board();
	Board(const Board& board);
	Board(const Board& board, BoardPos move, PlayerColor player);

	BoardData* getData();
	BoardSquare	getCase(int x, int y) const;
	BoardSquare	getCase(BoardPos pos) const;
	int 		getCapturedBlack() const;
	int 		getCapturedWhite() const;

	bool 		playCapture(int x, int y);
	bool 		playCaptureDir(int x, int y, int dirX, int dirY, BoardSquare type);
	bool 		isAlignedStoneDir(int x, int y, int dirX, int dirY, BoardSquare good, int size) const;
	bool		isAlignedStone(int size) const;
private:
	BoardData	_data;
	int			_capturedWhites;
	int			_capturedBlacks;

};


