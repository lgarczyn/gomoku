//
// Created by Louis GARCZYNSKI on 3/30/16.
//


#pragma once

#include "IAnalyzer.hpp"
#include "Board.hpp"
#include "BoardPos.hpp"

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
	int turn;
	int depth;
	MoveScore negamax(Board* node, int depth, Score alpha, Score beta, int player);

public:
	Game(bool isBrainDead);
	~Game();
	void play(BoardPos pos);
	void play();

	Board *getState();
	BoardPos getNextMove();

	bool 	playCapture(int x, int y);
	bool 	playCaptureDir(int x, int y, int dirX, int dirY, BoardSquare type);
};


