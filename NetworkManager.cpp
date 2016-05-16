//
// Created by Louis GARCZYNSKI on 4/13/16.
//

#include "NetworkManager.hpp"
#include "Constants.hpp"

const 	int NetworkManager::network_depth = 19;

void NetworkManager::setup_network(Network& network)
{
	network.setStructure(BOARD_WIDTH * BOARD_HEIGHT + 2, network_depth * 10, 1, shark::FFNetStructures::InputOutputShortcut, true);
}

bool NetworkManager::read_network(const char* path, Network& network)
{
	try
	{
		std::ifstream ifs(path);
		boost::archive::polymorphic_text_iarchive ia(ifs);
		network.read(ia);
		ifs.close();
		return true;
	}
	catch (std::exception e)
	{
		return false;
	}
}

Network NetworkManager::get_variations(Network value)
{
	value.setLayer(0, value.layerMatrix(0), value.bias(0));
	return value;
}

bool NetworkManager::write_network(const char* path, Network& network)
{
	try
	{
		std::ofstream ofs(path);
		boost::archive::polymorphic_text_oarchive oa(ofs);
		network.write(oa);
		ofs.close();
		return true;
	}
	catch (std::exception e)
	{
		return false;
	}
}