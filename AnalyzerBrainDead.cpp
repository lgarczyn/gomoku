//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "AnalyzerBrainDead.hpp"
#include "Board.hpp"

AnalyzerBrainDead::AnalyzerBrainDead() { }
AnalyzerBrainDead::~AnalyzerBrainDead() { }

/*inline void AnalyzerBrainDead::fillScoreDir(Board &board, int x, int y, int dirX, int dirY, BoardSquare color)
{
	const int maxX = CLAMP(x + 5 * dirX, -1, BOARD_WIDTH);
	const int maxY = CLAMP(y + 5 * dirY, -1, BOARD_HEIGHT);

	int value = 1;

	x += dirX, y+= dirY;
	while ((!dirX || x != maxX) && (!dirY || y != maxY))
	{
		BoardSquare square = board._data[y][x];
		if (square == empty)
		{
			_score += (color == BoardSquare::white) ? value : -value;
		}
		else if (square == color)
		{
			value <<= 2;
		}
		else
		{
			break;
		}
		x += dirX, y+= dirY;
	}
};

void AnalyzerBrainDead::fillScore(Board &board)
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			BoardSquare color = board._data[y][x];
			if (color != BoardSquare::empty)
			{
				fillScoreDir(board, x, y, -1, -1, color);
				fillScoreDir(board, x, y, -1, 0, color);
				fillScoreDir(board, x, y, -1, 1, color);
				fillScoreDir(board, x, y, 0, -1, color);
				//fillScoreDir(board, x, y, 0, 0, color);
				fillScoreDir(board, x, y, 0, 1, color);
				fillScoreDir(board, x, y, 1, -1, color);
				fillScoreDir(board, x, y, 1, 0, color);
				fillScoreDir(board, x, y, 1, 1, color);
			}
		}
	}
}*/

void AnalyzerBrainDead::fillScore(Board &board)
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			BoardSquare color = board._data[y][x];
			if (color != BoardSquare::empty)
			{
				for (int dirX = -1; dirX <= 1; dirX++)
				{
					for (int dirY = -1; dirY <= 1; dirY++)
					{
						if (dirX || dirY)
						{
							const int maxX = CLAMP(x + 5 * dirX, -1, BOARD_WIDTH);
							const int maxY = CLAMP(y + 5 * dirY, -1, BOARD_HEIGHT);

							int value = 1;

							int _x = x + dirX;
							int _y = y + dirY;

							while ((!dirX || _x != maxX) && (!dirY || _y != maxY))
							{
								BoardSquare square = board._data[_y][_x];
								if (square == empty)
								{
									_score += (color == BoardSquare::white) ? value : -value;
								}
								else if (square == color)
								{
									value <<= 2;
								}
								else
								{
									break;
								}
								_x += dirX, _y+= dirY;
							}
						}
					}
				}
			}
		}
	}
}

Score AnalyzerBrainDead::getScore(Board &board, bool considerCapture)
{
	_score = 0;
	
	fillScore(board);

	if (considerCapture)
	{
		_score += 1 << board._capturedBlacks / 2;
		_score -= 1 << board._capturedWhites / 2;
	}

	return _score;
}