//
// Created by Louis GARCZYNSKI on 4/13/16.
//


#pragma once

#include "IAnalyzer.hpp"

#define ANALYZER_AVAILABLE
#include<shark/Models/FFNet.h> //the feed forward neural network

class Trainer
{
private:
	shark::FFNet<
			shark::FastSigmoidNeuron,
			shark::LinearNeuron> network;
public:
	Trainer();
	~Trainer();
	void train();
};


