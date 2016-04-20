//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "Analyzer.hpp"

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
	if (board.hasScore)
		return board.score;

	int k = 0;
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			inputs.operator()(0, k++) = board.getCase(x, y);
		}
	}

	//TODO include captured whites and blacks
	//TODO include captured whites and blacks
	network.eval(inputs, outputs, *state);

	board.hasScore = true;
	board.score = outputs.operator()(0, 0);
	return (board.score);
}

Analyzer::~Analyzer()
{
	//delete allocated brain
}