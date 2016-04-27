//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#pragma once

#include "Board.hpp"
#include "IAnalyzer.hpp"
#include "NetworkManager.hpp"

#define ANALYZER_AVAILABLE
#include<shark/Models/FFNet.h> //the feed forward neural network
#include<shark/Algorithms/GradientDescent/Rprop.h> //resilient propagation as optimizer
#include<shark/ObjectiveFunctions/Loss/CrossEntropy.h> // loss during training
#include<shark/ObjectiveFunctions/ErrorFunction.h> //error function to connect data model and loss
#include<shark/ObjectiveFunctions/Loss/ZeroOneLoss.h> //loss for test performance

//evaluating probabilities
#include<shark/Models/Softmax.h> //transforms model output into probabilities
#include<shark/Models/ConcatenatedModel.h> //provides operator >> for concatenating models
#include<shark/Models/FFNet.h> //the feed forward neural network
#include<shark/Algorithms/GradientDescent/Rprop.h> //resilient propagation as optimizer
#include<shark/ObjectiveFunctions/Loss/CrossEntropy.h> // loss during training
#include<shark/ObjectiveFunctions/ErrorFunction.h> //error function to connect data model and loss
#include<shark/ObjectiveFunctions/Loss/ZeroOneLoss.h> //loss for test performance

//evaluating probabilities
#include<shark/Models/Softmax.h> //transforms model output into probabilities
#include<shark/Models/ConcatenatedModel.h> //provides operator >> for concatenating models


class Analyzer : public IAnalyzer
{
private:
	shark::RealMatrix inputs;
	shark::RealMatrix outputs;
	Network network;
	boost::shared_ptr<shark::State> state;
public:
	Analyzer();
	Score getScore(Board& board, bool considerCapture);
	~Analyzer();
};
