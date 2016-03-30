//
// Created by Louis GARCZYNSKI on 3/30/16.
//


#pragma once

#include "Board.hpp"
#include <limits>

using Score=int;
const int pinfinity = std::numeric_limits<Score>::max();
const int ninfinity = std::numeric_limits<Score>::max();

class IAnalyzer
{
public:
	virtual Score getScore(Board& board) = 0;
};


