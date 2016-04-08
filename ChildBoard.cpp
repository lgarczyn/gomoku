//
// Created by Louis GARCZYNSKI on 4/5/16.
//

#include "ChildBoard.hpp"

ChildBoard::ChildBoard():board(),move(){}
ChildBoard::ChildBoard(Board *_board, BoardPos _move):board(_board),move(_move){}