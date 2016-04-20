//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#pragma once

#include "Constants.hpp"

class Board;

class IAnalyzer
{
public:
	virtual Score getScore(Board& board) = 0;
};


