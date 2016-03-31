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
	turn = 1;
}

Game::~Game()
{

}

Game::MoveScore Game::negamax(Board* node, int depth, Score alpha, Score beta, int player)
{
	auto children = node->getChildren();
	//sort moves

	MoveScore bestMove(ninfinity);

	for (Board* child:children)
	{
		MoveScore move;

		if (child->isTerminal())
		{
			move = MoveScore(pinfinity, child->getMove());
		}
		else if (depth <= 1)
		{
			move = MoveScore(player * analyzer->getScore(*child), child->getMove());
		}
		else
		{
			move = negamax(child, depth - 1, -beta, -alpha, -player);
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

void Game::play(BoardPos pos)
{
	++turn;

	(*state->getData())[pos.y][pos.x] = turn & 1 ? BoardSquare::black : BoardSquare::white;
	playCapture(pos.x, pos.y);
}

void Game::play()
{
	play(getNextMove());
}

Board *Game::getState()
{
	return (state);
}

bool	Game::playCaptureDir(int x, int y, int dirX, int dirY, BoardSquare good) {

	BoardSquare bad = (good == BoardSquare::white ? BoardSquare::black : BoardSquare::white);

	if (x + 3*dirX < 0 || x + 3*dirX >= BOARD_WIDTH
			|| y + 3*dirY < 0 || y + 3*dirY >= BOARD_HEIGHT)
		return (false);
	return ((*state->getData())[y][x] == good
			&& (*state->getData())[y + dirY*1][x + dirX*1] == bad
			&& (*state->getData())[y + dirY*2][x + dirX*2] == bad
			&& (*state->getData())[y + dirY*3][x + dirX*3] == good);
}

bool 	Game::playCapture(int x, int y) {
	BoardSquare c = (*state->getData())[y][x];

	for (int dirX = -1 ; dirX <= 1; ++dirX)
		for (int dirY = -1 ; dirY <= 1; ++dirY)
			if (playCaptureDir(x, y, dirX, dirY, c)) {
				(*state->getData())[y + dirY*1][x + dirX*1] = c;
				(*state->getData())[y + dirY*2][x + dirX*2] = c;
				return (true);
			}
	return false;
}