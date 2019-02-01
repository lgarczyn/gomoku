//
// Created by Louis GARCZYNSKI on 3/30/16.
//


#pragma once

#include <ctime>
#include "PlayerColor.hpp"
#include "BoardPos.hpp"
#include "Constants.hpp"
#include "MoveScore.hpp"
#include "ChildBoard.hpp"
#include "ThreadData.hpp"
#include "Options.hpp"
#include <atomic>
#include <chrono>
#include <random>

template <typename Data, typename Value> class ThreadPool;

class Game
{
public:
	Game(const Options& _options);
	~Game();
	bool play(BoardPos pos);
	bool play();

	PlayerColor getTurn() const;
	bool isPlayerNext() const;
	bool hasPosChanged(BoardPos pos) const;
	bool isOverdue() const;
	double getTimeDiff() const;
	double getTimeTaken() const;
	int getMovesExplored() const;
	Score getCurrentScore() const;

	Board *getState();
	BoardPos getNextMove();

private:

	typedef ThreadPool<ThreadData, MoveScore> Pool;

	std::chrono::high_resolution_clock::time_point _start;
	std::mt19937 _randomDevice;

	Options				_options;
	double				_timeLimit;
	double				_timeTaken;
	std::atomic<int>	_movesExplored;
	int					_constDepth;
	int					_depth;

	Board*		_state;
	Board*		_previousState;
	PlayerColor	_turn;

	Score negamax(Board& node, int depth, Score alpha, Score beta, PlayerColor player);
	MoveScore negamaxThread(ThreadData data);
	BoardPos startNegamax(Board *node, PlayerColor player);

	Pool *_pool;
};


