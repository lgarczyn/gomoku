//
// Created by Louis GARCZYNSKI on 3/30/16.
//


#pragma once

#include "IAnalyzer.hpp"

class AnalyzerBrainDead:IAnalyzer
{
public:
	AnalyzerBrainDead();
	Score getScore(Board& board);
};


