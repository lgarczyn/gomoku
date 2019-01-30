//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "Analyzer.hpp"
#include "Board.hpp"

Analyzer::Analyzer() { }
Analyzer::~Analyzer() { }

Score Analyzer::fillScore(Board &board)
{
	Score score = 0;
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			BoardSquare color = board._data[y][x];
			if (color != BoardSquare::empty)
			{
				Score squareScore = 0;
				for (int dirX = -1; dirX <= 1; dirX++)
				{
					for (int dirY = -1; dirY <= 1; dirY++)
					{
						if (dirX || dirY)
						{
							const int maxX = CLAMP(x + 5 * dirX, -1, BOARD_WIDTH);
							const int maxY = CLAMP(y + 5 * dirY, -1, BOARD_HEIGHT);

							int value = 1;
							int emptyCount = 0;

							int _x = x + dirX;
							int _y = y + dirY;

							while ((dirX == 0 || _x != maxX) && (dirY == 0 || _y != maxY))
							{
								BoardSquare square = board._data[_y][_x];
								if (square == empty)
								{
									squareScore += value;
									emptyCount++;
								}
								else if (square == color)
								{
									value <<= 3;
								}
								else
								{
									break;
								}
								_x += dirX, _y+= dirY;
							}
							value >>= 2;
							squareScore += value * emptyCount;
						}
					}
				}
				score += (color == BoardSquare::white) ? squareScore : -squareScore;
			}
		}
	}
	return score;
}

Score Analyzer::getScore(Board &board, bool considerCapture)
{
	Score score = fillScore(board);

	if (considerCapture)
	{
		score += 1 << board._capturedBlacks / 2;
		score -= 1 << board._capturedWhites / 2;
	}
	if (board._victoryFlag == whitePlayer)
	{
		score += pinfinity / 2;
	}
	else if (board._victoryFlag == blackPlayer)
	{
		score -= pinfinity / 2;
	}

	return score;
}