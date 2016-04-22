//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "Game.hpp"
#include "Analyzer.hpp"
#include "Board.hpp"
#include <iostream>

using namespace std;

Game::Game(Options options):_options(options)
{
	_state = new Board();
#ifdef ANALYZER_AVAILABLE
	_analyzer = _options.brainDead ?
			   (IAnalyzer*)new AnalyzerBrainDead() :
			   (IAnalyzer*)new Analyzer();
#else
	_analyzer = new AnalyzerBrainDead();
#endif
	_turn = PlayerColor::blackPlayer;
	_depth = 4;
	_state->fillTaboo(_options.limitBlack, _options.doubleThree, _turn);
	_state->_priority[9][9] = 1;
}

Game::~Game()
{

}

MoveScore Game::negamax(Board* node, int negDepth, Score alpha, Score beta, PlayerColor player)
{
	//auto children = node->getChildren(player, _options.capture, _analyzer);
	auto children = node->getChildren(player, _options.capture, (negDepth == _depth) ? 100 : 10);

	MoveScore bestMove(ninfinity);
	std::vector<MoveScore>	choice;


	if (!children.size())
		throw std::logic_error("GetChildren returned an empty array");

	for (int i = 0; i < children.size(); i++)
	{
		ChildBoard child = children[i];
		int score;
		Board* board = child.board;
		BoardPos pos = child.move;

		if (board->isTerminal(_options.captureWin))
		{
			score = pinfinity;
		}
		else if (negDepth <= 1)
		{
			//board->fillTaboo(_options.limitBlack, _options.doubleThree, -player);
			score = player * _analyzer->getScore(*board);
		}
		else if (i == 0)
		{
			board->fillTaboo(_options.limitBlack, _options.doubleThree, -player);
			//score = -negamax(board, negaDepth - 1, -beta, -alpha, -player).score;

			score = -negamax(child.board, negDepth - 1, -beta, -alpha, -player).score;
		}
		else
		{
			score = -negamax(child.board, negDepth - 1, -alpha - 1, -alpha, -player).score;
			if (alpha < score && score < beta)
				score = -negamax(child.board, negDepth - 1, -beta, -score, -player).score;
		}
		if (score > bestMove.score)
		{
			choice.clear();
			bestMove = MoveScore(score, pos);
			choice.push_back(bestMove);
		}
		else if (score == bestMove.score)
		{
			bestMove = MoveScore(score, pos);
			choice.push_back(bestMove);
		}
		delete board;

		alpha = std::max(alpha, score);
		if (alpha > beta)
			break;
	}
	return choice[rand() % choice.size()];//TODO get fucking rid of rand
}

MoveScore Game::pvs(Board *node, int depth, int alpha, int beta, PlayerColor player)
{
	if (node->isTerminal(_options.capture))
		return ninfinity;
	if (depth == 0)
		return player * _analyzer->getScore(*node);

	auto children = node->getChildren(player, _options.capture, 10);

	MoveScore bestMove;
	for (int i = 0; i < children.size(); i++)
	{
		ChildBoard child = children[i];
		int score = ninfinity;
		if (i == 0)
		{
			score = -pvs(child.board, depth - 1, -beta, -alpha, -player).score;
		}
		else
		{
			score = -pvs(child.board, depth - 1, -alpha - 1, -alpha, -player).score;
			if (alpha < score && score < beta)
				score = -pvs(child.board, depth - 1, -beta, -score, -player).score;
		}
		if (alpha > score)
			bestMove = MoveScore(alpha, child.move);
		if (alpha >= beta)
			break;
	}

	return bestMove;
}

BoardPos Game::getNextMove()
{
	auto  pos = negamax(_state, _depth, ninfinity, pinfinity, -_turn).pos;
	return pos;
}

bool Game::play(BoardPos pos)
{
	if (_state->getCase(pos) != BoardSquare::empty)
		return false;
	if (_state->getPriority(pos) < 0)
		return false;

	Board* tmp = _state;
	_state = new Board(*_state, pos, _turn, _options.capture);
	delete tmp;
	_turn = -_turn;
	_state->fillTaboo(_options.limitBlack, _options.doubleThree, _turn);
	_state->fillPriority(_turn);
	return _state->isTerminal(_options.captureWin);
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