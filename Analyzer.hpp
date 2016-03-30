//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#pragma once

#define ANALYZER_AVAILABLE
#include "IAnalyzer.hpp"

class Analyzer: IAnalyzer
{
public:
	Analyzer();
	score getScore(Board& board);
	~Analyzer();
};
