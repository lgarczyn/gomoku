//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "Analyzer.hpp"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <iostream>
#include <fstream>

using namespace boost::archive;
const int Analyzer::network_depth = 19;

Analyzer::Analyzer()
{
	network.setStructure(BOARD_WIDTH * BOARD_HEIGHT, network_depth, 1, shark::FFNetStructures::Normal);

	try
	{
		std::ifstream ifs("networks/gomoku.model");
		boost::archive::polymorphic_text_iarchive ia(ifs);
		network.read(ia);
		ifs.close();
	}
	catch (std::exception e)
	{
		try
		{
			std::ofstream ofs("networks/gomoku.model");
			boost::archive::polymorphic_text_oarchive oa(ofs);
			network.write(oa);
			ofs.close();

		}
		catch (std::exception e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}

Score Analyzer::getScore(Board& board)
{
	return (0);
	//apply brain file
}

Analyzer::~Analyzer()
{
	//delete allocated brain
}