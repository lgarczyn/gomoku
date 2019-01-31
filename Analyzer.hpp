//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#pragma once

class Board;

#include "BoardSquare.hpp"
#include "Constants.hpp"

class Analyzer
{
public:
	static Score getScore(Board& board, bool considerCapture);

private:
	static Score fillScore(Board &board);
};


