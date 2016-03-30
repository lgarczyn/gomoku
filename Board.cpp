//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "Board.hpp"

Board::BoardPos::BoardPos() : x(0), y(0) {

};

Board::BoardPos::BoardPos(int _x, int _y): x(_x), y(_y) {

};

Board::Board(void) : _captured() {

};