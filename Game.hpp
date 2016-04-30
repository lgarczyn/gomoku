//
// Created by Louis GARCZYNSKI on 3/30/16.
//


#pragma once

#include "PlayerColor.hpp"
#include "BoardPos.hpp"
#include "IAnalyzer.hpp"
#include "Constants.hpp"
#include "MoveScore.hpp"

class Game
{
public:
	struct Options
	{
		bool showTips = true;
		bool showPriority = false;
		bool limitBlack = false;
		bool doubleThree = true;
		bool capture = true;
		bool captureWin = true;
		bool brainDead = true;
		bool isBlackAI = true;
		bool isWhiteAI = true;
	};

	Game(Options _options);
	~Game();
	bool play(BoardPos pos);
	bool play();

	PlayerColor getTurn() const;
	bool isPlayerNext() const;
	bool hasPosChanged(BoardPos pos) const;

	Board *getState();
	BoardPos getNextMove();

private:

	Options _options;
	Board *_state;
	Board *_previousState;
	IAnalyzer* _analyzer;
	PlayerColor _turn;
	int _depth;
	Score negamax(Board* node, int depth, Score alpha, Score beta, PlayerColor player);
	BoardPos negamax(Board* node, PlayerColor player);
	MoveScore pvs(Board* node, int depth, Score alpha, Score beta, PlayerColor player);
};


