//
// Created by Louis GARCZYNSKI on 3/30/16.
//


#pragma once

/*
#include "IAnalyzer.hpp"
#include "Board.hpp"
#include <limits.h>
#include <limits>
class MTDF
{
public:

	static int AlphaBetaWithMemory(Board n, int alpha , int beta , int d)
	{
		if (retrieve(n) != nullptr)// Transposition table lookup
		{
			if (n.lowerbound >= beta)
				return n.lowerbound;
			if (n.upperbound <= alpha)
				return n.upperbound;
			alpha = max(alpha, n.lowerbound);
			beta = min(beta, n.upperbound);
		}
		int g;
		if (d == 0)
			g = evaluate(n); // leaf node
		else if (n == MAXNODE)
		{
			g = -INFINITY;

			int a = alpha; // save original alpha value

			for (Board c:n.get_children())
			{
				if (g >=  beta)
					break;

				g = max(g, AlphaBetaWithMemory(c, a, beta, d - 1));
				a = max(a, g);
			}
		}
		else
		{
			g = +INFINITY;

			int b = beta;//  save original beta value
			for (Board c:n.get_children())
			{
				if (g <= alpha)
					break;
				g = min(g, AlphaBetaWithMemory(c, alpha, b, d - 1));
				b = min(b, g);
			}
		}
		// Traditional transposition table storing of bounds
		// Fail low result implies an upper bound
		if (g <= alpha)
		{
			n.upperbound = g;
			store(n.upperboung);
		}
		// Found an accurate minimax value - will not occur if called with zero window
		if (g > alpha and g < beta)
		{
			n.lowerbound = g;
			n.upperbound = g;
			store(n.lowerbound, n.upperbound);
		}
		// Fail high result implies a lower bound
		if (g >= beta)
		{
			n.lowerbound = g;
			store (n.lowerbound);
		}
		return g;
	}

	static int MDTF(Board board, int f, int d)
	{
		int g = f;
		int upperbound = std::numeric_limits<score>::max();
		int lowerbound = std::numeric_limits<score>::min();

		while (lowerbound < upperbound)
		{
			int beta;
			if (g == lowerbound)
				beta = g + 1;
			else
				beta = g;

			g = AlphaBetaWithMemory(board, beta - 1, beta, d);
			if (g < beta)
				upperbound = g;
			else
				lowerbound = g;
		}
		return g;
	}

	static int iterative_deepening(Board board, int depth)
	{
		int firstguess = 0;
		for (int d = 1; d < depth; d++)
		{
			firstguess = MDTF(board, firstguess, d);

			if (times_up())//implement no mo time
				break;
		}
		return firstguess;
	}
};
*/