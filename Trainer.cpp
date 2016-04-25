//
// Created by Louis GARCZYNSKI on 4/13/16.
//

#include "Trainer.hpp"
#include "NetworkManager.hpp"
#include "Board.hpp"
#include "AnalyzerBrainDead.hpp"
#include "BoardPos.hpp"
#include <shark/ObjectiveFunctions/Loss/SquaredLoss.h>

#include<shark/Algorithms/DirectSearch/ElitistCMA.h>
#include<shark/Algorithms/GradientDescent/Rprop.h> //resilient propagation as optimizer
#include<shark/ObjectiveFunctions/Loss/CrossEntropy.h> // loss during training
#include<shark/ObjectiveFunctions/ErrorFunction.h> //error function to connect data model and loss
#include<shark/ObjectiveFunctions/Loss/ZeroOneLoss.h> //loss for test performance

//evaluating probabilities
#include<shark/Models/Softmax.h> //transforms model output into probabilities
#include<shark/Models/ConcatenatedModel.h> //provides operator >> for concatenating models
#include<random>

Trainer::Trainer()
{
	NetworkManager::setup_network(network);
	if (!NetworkManager::read_network("networks/gomoku.model", network))
	{
		if (!NetworkManager::write_network("networks/gomoku.model", network))
		{
			std::cerr << "Could not read neural network";
			exit(1);
		}
	}
}
//http://image.diku.dk/shark/sphinx_pages/build/html/rest_sources/tutorials/algorithms/ffnet.html


shark::LabeledData<shark::RealVector, shark::RealVector> getRandomData(int count = 100)
{
	std::default_random_engine engine;

	engine.seed();
	std::uniform_int_distribution<> distribution(0, 10);


	//the 2D xor Problem has 4 patterns, (0,0), (0,1), (1,0), (1,1)
	std::vector<shark::RealVector> inputs(count, shark::RealVector(BOARD_WIDTH * BOARD_HEIGHT + 2));
	//the result is 1 if both inputs have a different value, and 0 otherwise
	std::vector<shark::RealVector> labels(count, shark::RealVector(1));
	IAnalyzer *analyzer = new AnalyzerBrainDead();

	for(unsigned i=0; i < count; i++){

		Board *board = new Board();
		BoardData* data = board->getData();

		for (BoardPos pos; pos != BoardPos::boardEnd; ++pos)
		{
			int r = distribution(engine);
			if (r == 11)
				board->getCase(pos) = BoardSquare::white;
			else if (r == 1)
				board->getCase(pos) = BoardSquare::black;
		}
		//board.fillTaboo(false, true, PlayerColor::blackPlayer);

		for (BoardPos pos; pos != BoardPos::boardEnd; ++pos)
		{
			inputs[i](pos.y * BOARD_HEIGHT + pos.x) = board->getCase(pos);
		}
		labels[i](0) = analyzer->getScore(*board) + 1000;
		delete board;
	}
	shark::LabeledData<shark::RealVector,shark::RealVector> dataset = shark::createLabeledDataFromRange(inputs,labels);
	return dataset;
}

void Trainer::train()
{
	//input == RealVector
	//label == unsigned int
	//output == RealVector

	//label input label
	//sqloss output label
	//model input output
	//errorfunc input label output


//create error function
	{
		shark::LabeledData<shark::RealVector, shark::RealVector> dataset = getRandomData(100);
		shark::SquaredLoss<shark::RealVector, shark::RealVector> loss; // surrogate loss for training

		shark::AbstractModel<shark::RealVector, shark::RealVector> *model = &network;
		shark::AbstractLoss<shark::RealVector, shark::RealVector> *aloss = &loss;

		shark::ErrorFunction error(dataset, model, aloss);

		//initialize Rprop and initialize the network randomly
		//initRandomUniform(network,-0.1,0.1);
		shark::IRpropPlus optimizer;
		//shark::ElitistCMA optimizer;

		optimizer.init(error);
		unsigned numberOfSteps = 1000;
		for (unsigned step = 0; step != numberOfSteps; ++step)
		{
			std::cout << step << std::endl;
			optimizer.step(error);
		}
	}
	{

		std::cout << "Test" << std::endl;


		IAnalyzer* analyzer = new AnalyzerBrainDead();

		for (int i = 0; i < 100; i++)
		{
			Board* board = new Board();
			for (BoardPos pos; pos != BoardPos::boardEnd; ++pos)
			{
				int r = rand() % 10;
				if (r == 0)
					board->getCase(pos) = BoardSquare::white;
				else if (r == 1)
					board->getCase(pos) = BoardSquare::black;
			}

			shark::RealMatrix input(1, BOARD_HEIGHT * BOARD_WIDTH);
			shark::RealMatrix output(1, 1);

			for (BoardPos pos; pos != BoardPos::boardEnd; ++pos)
			{
				input.operator()(0, pos.y * BOARD_HEIGHT + pos.x) = board->getCase(pos);
			}

			network.eval(input, output, *network.createState());

			std::cout << "Dataset score: " << analyzer->getScore(*board) << std::endl;
			std::cout << "Analyzed score: " << output.operator()(0, 0) - 1000 << std::endl;
		}
	}
}

//LOAD NETWORK

//GET NETWORK VARIATIONS //TODO create generation function, probably based on a normal-variation algo
//COMPARE NETWORKS SKILLZ //TODO possibility of giving networks to Game
//KEEP BEST NETWORKS

//WRITE NETWORKS

Trainer::~Trainer()
{
	NetworkManager::setup_network(network);
	if (!NetworkManager::write_network("networks/gomoku.model", network))
	{
		std::cerr << "Could not read neural network";
		exit(1);
	}
}
