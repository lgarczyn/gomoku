//
// Created by Louis GARCZYNSKI on 3/30/16.
//

class IAnalyzer;
using Score=int;

#pragma once

#include <limits>
#include "Board.hpp"

const int pinfinity = std::numeric_limits<Score>::max();
const int ninfinity = std::numeric_limits<Score>::min();

class IAnalyzer
{
public:
	virtual Score getScore(Board& board) = 0;
};


