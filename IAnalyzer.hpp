//
// Created by Louis GARCZYNSKI on 3/30/16.
//


#include <limits>

class IAnalyzer;
using Score=int;
const int pinfinity = std::numeric_limits<Score>::max();
const int ninfinity = std::numeric_limits<Score>::max();

#pragma once

#include "Board.hpp"

class IAnalyzer
{
public:
	virtual Score getScore(Board& board) = 0;
};


