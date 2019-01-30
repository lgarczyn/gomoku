//
// Created by Louis GARCZYNSKI on 3/30/16.
//

class IAnalyzer;

#pragma once

#include "IAnalyzer.hpp"
#include "BoardSquare.hpp"

class Analyzer : public IAnalyzer
{
public:
	Analyzer();
	virtual ~Analyzer();
	Score getScore(Board& board, bool considerCapture);

private:
	Score fillScore(Board &board);
};


