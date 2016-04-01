//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "Game.hpp"
#include "Analyzer.hpp"
#include "AnalyzerBrainDead.hpp"

Game::Game(bool isBrainDead)
{
	_state = new Board();
#ifdef ANALYZER_AVAILABLE
	_analyzer = isBrainDead ?
			   (IAnalyzer*)new AnalyzerBrainDead() :
			   (IAnalyzer*)new Analyzer();
#else
	_analyzer = new AnalyzerBrainDead();
#endif
	_turn = PlayerColor::whitePlayer;
	_depth = 2;
}

Game::~Game()
{

}

Game::MoveScore Game::negamax(Board* node, int negaDepth, Score alpha, Score beta, PlayerColor player)
{
	auto children = node->getChildren(player);
	//TODO sort moves

	MoveScore bestMove(ninfinity);

	for (ChildBoard child : children)
	{
		MoveScore move;
		Board* board = std::get<0>(child);
		BoardPos pos = std::get<1>(child);

		if (board->isTerminal())
		{
			move = MoveScore(pinfinity, pos);
		}
		else if (negaDepth <= 1)
		{
			move = MoveScore(player * _analyzer->getScore(*board), pos);
		}
		else
		{
			move.score = -negamax(board, negaDepth - 1, -beta, -alpha, -player).score;
			move.pos = pos;
		}
		if (move.score > bestMove.score || (move.score == bestMove.score && player == -1))
		{
			bestMove = move;
		}

		alpha = std::max( alpha, move.score);
		if (alpha > beta)
			break;
	}
	return bestMove;
}

#include <iostream>

using  namespace std;

BoardPos Game::getNextMove()
{
	auto  pos = negamax(_state, _depth, ninfinity, pinfinity, -_turn).pos;
	cout << pos.x << " : " << pos.y << endl;
	return pos;
}

bool Game::play(BoardPos pos)
{
	if (_state->getCase(pos) != BoardSquare::empty)
		return false;

	_state = new Board(*_state, pos, _turn);
	_turn = -_turn;
	return true;
}

bool Game::play()
{
	return play(getNextMove());
}

Board *Game::getState()
{
	return (_state);
}

PlayerColor Game::getTurn() const
{
	return  (_turn);
}