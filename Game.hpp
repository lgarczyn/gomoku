//
// Created by Louis GARCZYNSKI on 3/30/16.
//


#pragma once

#include "IAnalyzer.hpp"

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
	void play(BoardPos pos);
	void play();

	Board *getState();
	BoardPos getNextMove();
};


