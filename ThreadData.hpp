//
// Created by Louis GARCZYNSKI on 5/3/16.
//

#pragma once

#include "ChildBoard.hpp"
#include "PlayerColor.hpp"
#include <atomic>

class Game;

struct ThreadData
{
	ThreadData(){};
	ThreadData(ChildBoard _node, std::atomic<Score>* _alpha, PlayerColor _player):
			node(_node),
			alpha(_alpha),
			player(_player)
	{}

	~ThreadData(){};
	ChildBoard node;
	std::atomic<Score>* alpha;
	PlayerColor player;
};
