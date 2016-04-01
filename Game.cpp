//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "Game.hpp"
#include "Analyzer.hpp"
#include "AnalyzerBrainDead.hpp"

Game::Game(bool isBrainDead)
{
	state = new Board();
#ifdef ANALYZER_AVAILABLE
	analyzer = isBrainDead ?
			   (IAnalyzer*)new AnalyzerBrainDead() :
			   (IAnalyzer*)new Analyzer();
#else
	analyzer = new AnalyzerBrainDead();
#endif
	turn = PlayerColor::whitePlayer;
	depth = 2;
}

Game::~Game()
{

}

Game::MoveScore Game::negamax(Board* node, int negaDepth, Score alpha, Score beta, PlayerColor player)
{
	auto children = node->getChildren(player);
	//TODO sort moves

	MoveScore bestMove(ninfinity);

	for (Board* child:children)
	{
		MoveScore move;

		if (child->isTerminal())
		{
			move = MoveScore(pinfinity, child->getMove());
		}
		else if (negaDepth <= 1)
		{
			move = MoveScore(player * analyzer->getScore(*child), child->getMove());
		}
		else
		{
			move = negamax(child, negaDepth - 1, -beta, -alpha, -player);
			move.score = -move.score;
		}
		if (move.score > bestMove.score)
		{
			bestMove = move;
		}

		alpha = std::max( alpha, move.score);
		if (alpha > beta)
			break;
	}
	return bestMove;
}

BoardPos Game::getNextMove()
{
	return negamax(state, depth, ninfinity, pinfinity, turn).pos;
}

bool Game::play(BoardPos pos)
{
	if (state->getCase(pos) != BoardSquare::empty)
		return false;

	state = new Board(*state, pos, turn);
	turn = -turn;
	return true;
}

bool Game::play()
{
	return play(getNextMove());
}

Board *Game::getState()
{
	return (state);
}