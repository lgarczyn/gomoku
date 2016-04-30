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
	_depth = 5;
	_state->fillTaboo(_options.limitBlack, _options.doubleThree, _turn);
	_previousState = nullptr;
}

Game::~Game()
{

}

Score Game::negamax(Board* node, int negDepth, Score alpha, Score beta, PlayerColor player)
{
	auto children = node->getChildren(player, _options.capture, 20);

	Score bestScore = ninfinity;

	if (!children.size())
		throw std::logic_error("GetChildren returned an empty array");

	for (int i = 0; i < children.size(); i++)
	{
		ChildBoard child = children[i];
		Score score;
		Board* board = child.board;
		BoardPos pos = child.move;

		if (board->isTerminal(pos, _options.captureWin))
		{
			score = pinfinity - _depth + negDepth;
		}
		else if (negDepth <= 1)
		{
			score = player * _analyzer->getScore(*board, _options.captureWin);
		}
		else
		{
			score = -negamax(child.board, negDepth - 1, -beta, -alpha, -player);
		}
		if (score > bestScore)
		{
			bestScore = score;
		}
		delete board;

		alpha = std::max(alpha, score);
		if (alpha > beta)
			break;
	}
	return bestScore;
}

BoardPos Game::negamax(Board* node, PlayerColor player)
{
	Score alpha = ninfinity;
	Score beta = pinfinity;
	auto children = node->getChildren(player, _options.capture, 20);

	MoveScore bestMove(ninfinity - 1);
	std::vector<MoveScore>	choice;

	if (!children.size())
		throw std::logic_error("GetChildren returned an empty array");

	for (int i = 0; i < children.size(); i++)
	{
		ChildBoard child = children[i];
		Score score;
		Board* board = child.board;
		BoardPos pos = child.move;

		if (board->isTerminal(pos, _options.captureWin))
		{
			score = pinfinity;
		}
		else
		{
			score = -negamax(child.board, _depth - 1, -beta, -alpha, -player);
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
	return choice[rand() % choice.size()].pos;//TODO get fucking rid of rand
}

BoardPos Game::getNextMove()
{
	auto  pos = negamax(_state, _turn);
	return pos;
}

bool Game::play(BoardPos pos)
{
	if (_state->getCase(pos) != BoardSquare::empty)
		return false;
	if (_state->getPriority(pos) < 0)
		return false;

	delete _previousState;
	_previousState = _state;
	_state = new Board(*_previousState, pos, _turn, _options.capture);

	_turn = -_turn;
	_state->fillTaboo(_options.limitBlack, _options.doubleThree, _turn);
	_state->fillPriority(_turn);

	return _state->isTerminal(_options.captureWin);
}

bool Game::hasPosChanged(BoardPos pos) const
{
	if (_previousState == nullptr)
		return false;
	return _previousState->getCase(pos) != _state->getCase(pos);
}

bool Game::play()
{
	return play(getNextMove());
}

bool Game::isPlayerNext() const
{
	return
		(!_options.isBlackAI && getTurn() == blackPlayer) ||
		(!_options.isWhiteAI && getTurn() == whitePlayer);
}

Board *Game::getState()
{
	return (_state);
}

PlayerColor Game::getTurn() const
{
	return  (_turn);
}