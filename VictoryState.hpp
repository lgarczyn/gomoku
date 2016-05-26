//
// Created by Eric DELANGHE on 4/2/16.
//

#ifndef GOMOKU_VICTORYSTATE_HPP
#define GOMOKU_VICTORYSTATE_HPP

enum VictoryType
{
	novictory = 0,
	aligned = 1,
	captured = 2,
	staleMate = 3,
};

struct VictoryState
{
	PlayerColor victor;
	VictoryType type;

	VictoryState(PlayerColor _victor, VictoryType _type):victor(_victor),type(_type){};
	VictoryState(VictoryType _type):victor(nullPlayer),type(_type){};
	VictoryState():victor(nullPlayer),type(novictory){};
};

#endif //GOMOKU_VICTORYSTATE_HPP
