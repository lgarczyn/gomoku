//
// Created by Louis GARCZYNSKI on 3/30/16.
//

class IAnalyzerBrainDead;

#pragma once

#include "IAnalyzer.hpp"
#include "BoardSquare.hpp"

class AnalyzerBrainDead : public IAnalyzer
{
public:
	AnalyzerBrainDead();
	virtual ~AnalyzerBrainDead();
	Score getScore(Board& board, bool considerCapture);

private:
	void fillScore(Board &board);
	//void fillScoreDir(Board &board, int x, int y, int dirX, int dirY, BoardSquare color);
	Score _score;
};


