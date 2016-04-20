//
// Created by Louis GARCZYNSKI on 3/30/16.
//

class IAnalyzerBrainDead;

#pragma once

#include "IAnalyzer.hpp"

class AnalyzerBrainDead : public IAnalyzer
{
public:
	AnalyzerBrainDead();
	Score getScore(Board& board);
};


