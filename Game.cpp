//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "Game.hpp"
#include "Analyzer.hpp"
#include "AnalyzerBrainDead.hpp"

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
	_depth = 2;
	_state->fillTaboo(_options.limitBlack, _options.doubleThree, _turn);
}

Game::~Game()
{

}

Game::MoveScore Game::negamax(Board* node, int negaDepth, Score alpha, Score beta, PlayerColor player)
{
	auto children = node->getChildren(player, _options.capture);
	//TODO sort moves

	MoveScore bestMove(ninfinity);

	for (ChildBoard child : children)
	{
		MoveScore move;
		Board* board = std::get<0>(child);
		BoardPos pos = std::get<1>(child);

		if (board->isTerminal(_options.captureWin))
		{
			move = MoveScore(pinfinity, pos);
		}
		else if (negaDepth <= 1)
		{
			board->fillTaboo(_options.limitBlack, _options.doubleThree, -player);
			move = MoveScore(player * _analyzer->getScore(*board), pos);
		}
		else
		{
			board->fillTaboo(_options.limitBlack, _options.doubleThree, -player);
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

BoardPos Game::getNextMove()
{
	auto  pos = negamax(_state, _depth, ninfinity, pinfinity, -_turn).pos;
	return pos;
}

bool Game::play(BoardPos pos)
{
	if (_state->getCase(pos) != BoardSquare::empty)
		return false;

	_state = new Board(*_state, pos, _turn, _options.capture);
	_turn = -_turn;
	_state->fillTaboo(_options.limitBlack, _options.doubleThree, _turn);
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