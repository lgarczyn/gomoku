//
// Created by Louis GARCZYNSKI on 4/20/16.
//


#pragma once

#include "Constants.hpp"
#include "BoardPos.hpp"

struct MoveScore
{
	Score score;
	BoardPos pos;

	MoveScore(Score _score, BoardPos _pos):score(_score), pos(_pos){}
	MoveScore(Score _score):pos(),score(_score){}
	MoveScore():pos(),score(){}
};


