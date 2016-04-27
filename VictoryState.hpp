//
// Created by Eric DELANGHE on 4/2/16.
//

#ifndef GOMOKU_VICTORYSTATE_HPP
#define GOMOKU_VICTORYSTATE_HPP

enum VictoryState
{
	novictory = 0,
	aligned = 1,
	whitesCaptured = 2,
	blacksCaptured = 3,
	staleMate = 4,
};

#endif //GOMOKU_VICTORYSTATE_HPP
