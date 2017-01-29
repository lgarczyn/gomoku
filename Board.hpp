//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#ifndef BOARD_HPP
# define BOARD_HPP

class Board;

#pragma once

#include <vector>
#include <tuple>

#include "Constants.hpp"
#include "BoardPos.hpp"
#include "PlayerColor.hpp"
#include "BoardSquare.hpp"
#include "VictoryState.hpp"
#include "ChildBoard.hpp"
#include "MoveScore.hpp"
#include "Options.hpp"

using BoardData = BoardSquare[BOARD_HEIGHT][BOARD_WIDTH];
using BoardScore = short[BOARD_HEIGHT][BOARD_WIDTH];

class Board
{
	friend class AnalyzerBrainDead;
public:
	Board(PlayerColor player);
	Board(const Board& board);
	Board(const Board& board, BoardPos move, PlayerColor player, const Options& options);
	virtual ~Board();

	//VictoryState isTerminal(bool considerCapture);

	VictoryState	getVictory();
	size_t			getChildren(MoveScore* buffer, size_t count);

	void 			fillTaboo(bool limitBlack, bool doubleThree, PlayerColor player);
	void 			fillPriority(PlayerColor player, const Options& options);
	MoveScore		getBestPriority() const;
	bool			checkFreeThree(int x, int y, int dirX, int dirY, BoardSquare enemy);
	int 			playCapture(int x, int y);
	bool			isAlignedStone(int size) const;

    Score           fillScore();
    Score           getScore(bool considerCapture);

//Declaration here for optimization

	BoardData*		getData() { return &_data; }
	BoardSquare		getCase(BoardPos pos) const { return (_data[pos.y][pos.x]); }
	BoardSquare&	getCase(BoardPos pos) { return (_data[pos.y][pos.x]); }
	BoardSquare		getCase(int x, int y) const {return (_data[y][x]);};
	BoardSquare&	getCase(int x, int y) {return (_data[y][x]);};
	int 			getCapturedBlack() const {return (_capturedBlacks);}
	int 			getCapturedWhite() const {return (_capturedWhites);}
	int 			getPriority(int x, int y) const {return _priority[y][x];}
	int 			getPriority(BoardPos pos) const {return _priority[pos.y][pos.x];}
	bool 			isFlaggedFinal() const { return _victoryFlag; };

private:
	BoardData		_data;
	BoardScore		_priority;
	int				_capturedWhites;
	int				_capturedBlacks;
	int 			_turnNum;
	PlayerColor 	_turn;
	PlayerColor 	_victoryFlag;
	BoardPos		_alignmentPos;
	VictoryState	_victoryState;

	VictoryState	calculateVictory(BoardPos pos, const Options& options);

	void 			fillPriorityDir(int x, int y, int dirX, int dirY, BoardSquare color);//, int bonus);
	void 			fillCapturePriorityDir(int x, int y, int dirX, int dirY, BoardSquare color);

	bool 			isAlignedStoneDir(int x, int y, int dirX, int dirY, BoardSquare good, int size) const;
	bool		 	isAlignedStonePos(int x, int y, int size) const;
	bool 			playCaptureDir(int x, int y, int dirX, int dirY, BoardSquare type);
};

#include "Board.cpp"

#endif