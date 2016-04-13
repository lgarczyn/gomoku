//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "Analyzer.hpp"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <iostream>
#include <fstream>

Analyzer::Analyzer()
{
	inputs = shark::RealMatrix(1, BOARD_HEIGHT * BOARD_WIDTH);
	outputs = shark::RealMatrix(1, 1);

	NetworkManager::setup_network(network);
	if (!NetworkManager::read_network("networks/gomoku.model", network))
	{
		std::cerr << "Could not read neural network";
		exit(1);
	}

	state = network.createState();
}

Score Analyzer::getScore(Board& board)
{
	shark::State test;

	//TODO include captured whites and blacks
	//TODO actually copy the input in inputs
	network.eval(inputs, outputs, *state);
	return (outputs.operator()(0, 0));
}

Analyzer::~Analyzer()
{
	//delete allocated brain
}