//
// Created by Louis GARCZYNSKI on 3/30/16.
//

class IAnalyzer;

#pragma once

#include "Board.hpp"
#include "Constants.hpp"

class IAnalyzer
{
public:
	virtual Score getScore(Board& board) = 0;
};


