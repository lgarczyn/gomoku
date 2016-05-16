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

	ChildBoard():board(),move(){};
	ChildBoard(Board *_board, BoardPos _move):board(_board),move(_move){};
};


