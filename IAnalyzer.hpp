//
// Created by Louis GARCZYNSKI on 3/30/16.
//


#pragma once

#include "Board.hpp"
#include <limits>

using score=int;
const int pinfinity = std::numeric_limits<score>::max();
const int ninfinity = std::numeric_limits<score>::max();

class IAnalyzer
{
public:
	virtual score getScore(Board& board) = 0;
};


