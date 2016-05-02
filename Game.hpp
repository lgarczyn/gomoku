//
// Created by Louis GARCZYNSKI on 3/30/16.
//


#pragma once

#include <ctime>
#include "PlayerColor.hpp"
#include "BoardPos.hpp"
#include "IAnalyzer.hpp"
#include "Constants.hpp"
#include "MoveScore.hpp"
#include "ChildBoard.hpp"
#include "ThreadPool.hpp"
#include "ThreadData.hpp"
#include <atomic>

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
	bool isOverdue();
	double getTimeTaken() const;

	Board *getState();
	BoardPos getNextMove();

private:

	typedef ThreadPool<ThreadData, MoveScore> Pool;
	static const int threadCount = 8;

	clock_t _start;
	double 	_timeTaken;
	Options _options;
	Board *_state;
	Board *_previousState;
	IAnalyzer* _analyzer;
	PlayerColor _turn;
	int _depth;
	Score negamax(Board* node, int depth, Score alpha, Score beta, PlayerColor player);
	static MoveScore negamax_thread(ThreadData data);
	BoardPos start_negamax(Board *node, PlayerColor player);

	Pool *_pool;
};


