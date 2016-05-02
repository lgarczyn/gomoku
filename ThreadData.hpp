//
// Created by Louis GARCZYNSKI on 5/3/16.
//

#pragma once

#include "ChildBoard.hpp"
#include "PlayerColor.hpp"

class Game;

struct ThreadData
{
	ThreadData(ChildBoard _node, std::atomic<Score>* _alpha, PlayerColor _player, Game* _game):
			node(_node),
			alpha(_alpha),
			player(_player),
			game(_game)
	{}


	~ThreadData(){};
	ChildBoard node;
	std::atomic<Score>* alpha;
	PlayerColor player;
	Game* game;
};
