//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "Analyzer.hpp"

Analyzer::Analyzer()
{
	inputs = shark::RealMatrix(1, BOARD_HEIGHT * BOARD_WIDTH + 2);
	outputs = shark::RealMatrix(1, 1);

	NetworkManager::setup_network(network);
	if (!NetworkManager::read_network("networks/gomoku.model", network))
	{
		std::cerr << "Could not read neural network";
		exit(1);
	}

	state = network.createState();
}

Score Analyzer::getScore(Board& board, bool considerCapture)
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
	inputs(0, k) = considerCapture ? board.getCapturedWhite() : 0;
	inputs(0, ++k) = considerCapture ? board.getCapturedBlack() : 0;

	//TODO include captured whites and blacks
	network.eval(inputs, outputs, *state);

	board.score = network.operator()(shark::RealVector(BOARD_HEIGHT * BOARD_WIDTH + 2)).operator()(0);

	board.hasScore = true;
	board.score = outputs.operator()(0, 0);
	return (board.score);
}

Analyzer::~Analyzer()
{
	//delete allocated brain
}