//
// Created by Louis GARCZYNSKI on 3/30/16.
//


#pragma once

#include "BoardPos.hpp"
#include "IAnalyzer.hpp"
#include "Constants.hpp"

class Game
{
public:
	struct Options
	{
		bool limitBlack = false;//TODO
		bool doubleThree = true;//TODO
		bool capture = true;
		bool captureWin = true;
		bool brainDead = true;
	};

	Game(Options _options);
	~Game();
	bool play(BoardPos pos);
	bool play();

	PlayerColor getTurn() const;
	Board *getState();
	BoardPos getNextMove();

private:
	struct MoveScore
	{
		Score score;
		BoardPos pos;

		MoveScore(Score _score, BoardPos _pos):score(_score), pos(_pos){}
		MoveScore(Score _score):pos(),score(_score){}
		MoveScore():pos(),score(){}
	};

	Options _options;
	Board *_state;
	IAnalyzer* _analyzer;
	PlayerColor _turn;
	int _depth;
	MoveScore negamax(Board* node, int depth, Score alpha, Score beta, PlayerColor player);
};


