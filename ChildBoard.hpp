//
// Created by Louis GARCZYNSKI on 4/5/16.
//



#pragma once

#include "BoardPos.hpp"

class Board;

struct ChildBoard
{
	Board *board;
	BoardPos move;

	ChildBoard();
	ChildBoard(Board *_board, BoardPos _move);
};


