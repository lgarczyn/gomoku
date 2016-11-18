//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#pragma once

class IAnalyzerBrainDead;

#include "IAnalyzer.hpp"
#include "BoardSquare.hpp"

class AnalyzerBrainDead : public IAnalyzer
{
public:
	AnalyzerBrainDead();
	virtual ~AnalyzerBrainDead();
	Score getScore(Board& board, bool considerCapture);

private:
	Score fillScore(Board &board);
};


