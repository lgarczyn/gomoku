//
// Created by Louis GARCZYNSKI on 3/30/16.
//

class Board;

#pragma once

#include <vector>
#include <tuple>

#include "AnalyzerBrainDead.hpp"
#include "Constants.hpp"
#include "BoardPos.hpp"
#include "PlayerColor.hpp"
#include "BoardSquare.hpp"
#include "VictoryState.hpp"
#include "ChildBoard.hpp"
#include "MoveScore.hpp"

using BoardData = BoardSquare[BOARD_HEIGHT][BOARD_WIDTH];
using BoardScore = short[BOARD_HEIGHT][BOARD_WIDTH];

class Board
{
	friend class AnalyzerBrainDead;
public:

	VictoryState isTerminal(bool considerCapture);
	bool isPosInterest(int x, int y, PlayerColor player) const;
	std::vector<ChildBoard> getChildren(PlayerColor player, bool capture, int count);
	Board();
	Board(const Board& board);
	Board(const Board& board, BoardPos move, PlayerColor player, bool capture);

	BoardData*		getData();
	BoardSquare&	getCase(BoardPos pos);
	BoardSquare&	getCase(int x, int y);
	BoardSquare 	getCase(BoardPos pos) const;
	BoardSquare		getCase(int x, int y) const;
	int 			getPriority(int x, int y) const;
	int 			getPriority(BoardPos pos) const;
	MoveScore		getBestPriority() const;
	int 			getCapturedBlack() const;
	int 			getCapturedWhite() const;

	void 			fillTaboo(bool limitBlack, bool doubleThree, PlayerColor player);
	void 			fillPriority(PlayerColor player);
	void 			fillPriorityDir(int x, int y, int dirX, int dirY, BoardSquare color, int bonus);
	bool 			isPosLegal(int x, int y, bool limitBlack, bool doubleThree, PlayerColor player);
	bool			checkFreeThree(int x, int y, int dirX, int dirY, BoardSquare enemy);
	bool 			playCapture(int x, int y);
	bool 			playCaptureDir(int x, int y, int dirX, int dirY, BoardSquare type);
	bool 			isAlignedStoneDir(int x, int y, int dirX, int dirY, BoardSquare good, int size) const;
	bool			isAlignedStone(int size) const;

	bool 			hasScore;
	float 			score;
	BoardData		_data;
	BoardScore		_priority;
private:
	int				_capturedWhites;
	int				_capturedBlacks;
	int 			_turnNum;
	//friend class Analyzer;
};


