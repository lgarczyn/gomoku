//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "Game.hpp"
#include "Analyzer.hpp"
#include "AnalyzerBrainDead.hpp"

Game::Game(bool isBrainDead)
{
	state = Board();
#ifdef ANALYZER_AVAILABLE
	analyzer = isBrainDead ?
			   (IAnalyzer*)new AnalyzerBrainDead() :
			   (IAnalyzer*)new Analyzer();
#else
	analyzer = new AnalyzerBrainDead();
#endif
	turn = Board::white;

}

Game::~Game()
{

}

score negamax(Board node, int depth, score alpha, score beta, int color)
{
	if (node.isTerminal())
		return pinfinity;
	if (depth == 0)
		return color *

	childNodes := GenerateMoves(node)
	childNodes := OrderMoves(childNodes)
	bestValue := −∞
	foreach child in childNodes
	v := −negamax(child, depth − 1, −β, −α, −color)
	bestValue := max( bestValue, v )
	α := max( α, v )
	if α ≥ β
	break
	return bestValue
}

Board::BoardPos Game::getNextMove(int depth)
{
}

void Game::play(Board::BoardPos pos)
{

}

void Game::play()
{

}

Board Game::getState()
{
	return (state);
}
