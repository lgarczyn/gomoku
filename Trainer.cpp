//
// Created by Louis GARCZYNSKI on 4/13/16.
//

#include "Trainer.hpp"
#include "NetworkManager.hpp"
#include "AnalyzerBrainDead.hpp"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <iostream>
#include <fstream>
#include <vector>

Trainer::Trainer()
{
	NetworkManager::setup_network(network);
	if (!NetworkManager::read_network("networks/gomoku.model", network))
	{
		std::cerr << "Could not read neural network";
		exit(1);
	}
}
//http://image.diku.dk/shark/sphinx_pages/build/html/rest_sources/tutorials/algorithms/ffnet.html


shark::LabeledData<shark::RealVector,unsigned int> getRandomData()
{
	//the 2D xor Problem has 4 patterns, (0,0), (0,1), (1,0), (1,1)
	std::vector<shark::RealVector> inputs(100, shark::RealVector(BOARD_WIDTH * BOARD_HEIGHT));
	//the result is 1 if both inputs have a different value, and 0 otherwise
	std::vector<unsigned int> labels(100);
	IAnalyzer *analyzer = new AnalyzerBrainDead();

	for(unsigned i=0; i < 100; i++){

		Board board;
		BoardData* data = board.getData();

		for (BoardPos pos; pos != BoardPos::boardEnd; ++pos)
		{
			int r = rand() % 10;
			if (r == 0)
				board.getCase(pos) = BoardSquare::white;
			else if (r == 1)
				board.getCase(pos) = BoardSquare::black;
		}
		//board.fillTaboo(false, true, PlayerColor::blackPlayer);

		BoardPos pos;
		for (BoardPos pos; pos != BoardPos::boardEnd; ++pos)
		{
			inputs[i](pos.y * BOARD_HEIGHT + pos.x) = board.getCase(pos);
		}
		labels[i] = analyzer->getScore(board);
	}
	shark::LabeledData<shark::RealVector,unsigned int> dataset = shark::createLabeledDataFromRange(inputs,labels);
	return dataset;
}

void Trainer::train()
{
	//get problem data
	shark::LabeledData<shark::RealVector,unsigned int> dataset = getRandomData();

//create error function
	shark::CrossEntropy loss; // surrogate loss for training
	shark::ErrorFunction error(dataset,&network,&loss);

//initialize Rprop and initialize the network randomly
	initRandomUniform(network,-0.1,0.1);
	shark::IRpropPlus optimizer;
	optimizer.init(error);
	unsigned numberOfSteps = 1000;
	for(unsigned step = 0; step != numberOfSteps; ++step)
		optimizer.step(error);
}

Trainer::~Trainer()
{
	NetworkManager::setup_network(network);
	if (!NetworkManager::write_network("networks/gomoku.model", network))
	{
		std::cerr << "Could not read neural network";
		exit(1);
	}
}
