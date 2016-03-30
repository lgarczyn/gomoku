//
// Created by Louis GARCZYNSKI on 3/30/16.
//


#pragma once

#include "Board.hpp"
#include "IAnalyzer.hpp"

class Game
{
private:
	Board state;
	IAnalyzer* analyzer;
	Board::BoardSquare turn;
public:
	Game(bool isBrainDead);
	~Game();
	Board::BoardPos getNextMove();
	void play(Board::BoardPos pos);
	void play();
	Board getState();
};


