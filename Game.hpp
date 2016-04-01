//
// Created by Louis GARCZYNSKI on 3/30/16.
//


#pragma once

#include "BoardPos.hpp"
#include "IAnalyzer.hpp"
#include "Constants.hpp"

class Game
{
private:

	struct MoveScore
	{
		Score score;
		BoardPos pos;

		MoveScore(Score _score, BoardPos _pos):score(_score), pos(_pos){}
		MoveScore(Score _score):pos(),score(_score){}
		MoveScore():pos(),score(){}
	};

	Board *state;
	IAnalyzer* analyzer;
	PlayerColor turn;
	int depth;
	MoveScore negamax(Board* node, int depth, Score alpha, Score beta, PlayerColor player);

public:
	Game(bool isBrainDead);
	~Game();
	bool play(BoardPos pos);
	bool play();

	Board *getState();
	BoardPos getNextMove();
};


