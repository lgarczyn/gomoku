//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "AnalyzerBrainDead.hpp"
#include "Board.hpp"

AnalyzerBrainDead::AnalyzerBrainDead() { }

int countAlignedStoneDir(Board& data, int x, int y, int dirX, int dirY, BoardSquare good, int size)
{
	int i;
	int bonus = 0;

	if (x - dirX >= 0 && x - dirX < BOARD_WIDTH
		&& y - dirY >= 0 && y - dirY < BOARD_HEIGHT)
	{
		auto c = data._data[y - dirY][x - dirX];
		if (c == good)
			return (0);
		else if (c == empty)
			bonus += 1;
	}

	for (i = 0 ; i < size ; ++i) {
		if (x + i * dirX < 0 || x + i * dirX >= BOARD_WIDTH
			|| y + i * dirY < 0 || y + i * dirY >= BOARD_HEIGHT)
			return (i + bonus);
		if (data._data[y + dirY*i][x + dirX*i] != good)
			return (i + bonus);
	}
	return (i + bonus);
}

Score countAlignedStone(Board& data, int size)
{
	static const int score[] = {1, 2, 4, 8, 16, 32, 64};
	Score	sw = 0;
	Score   sb = 0;

	for (int y = 0 ; y < BOARD_HEIGHT; ++y)
		for (int x = 0 ; x < BOARD_WIDTH; ++x) {
			for (int dirX = -1; dirX <= 1; ++dirX)
				for (int dirY = -1; dirY <= 1; ++dirY)
					if (dirX || dirY) {
						auto c = data._data[y][x];

						if (c == BoardSquare::white || c == BoardSquare::black) {
							int dist = countAlignedStoneDir(data, x, y, dirX, dirY, c, size);

							if (c == BoardSquare::white)
								sw += dist * dist;
							else
								sb += dist * dist;
						}
					}
		}

	for (int y = 0 ; y < BOARD_HEIGHT; ++y)
		for (int x = 0 ; x < BOARD_WIDTH; ++x) {
			auto c = data._data[y][x];
			if (c == BoardSquare::white)
			{
				sw += score[countAlignedStoneDir(data, x, y, 1, 1, c, size)];
				sw += score[countAlignedStoneDir(data, x, y, 1, 0, c, size)];
				sw += score[countAlignedStoneDir(data, x, y, 1, -1, c, size)];
				sw += score[countAlignedStoneDir(data, x, y, 0, 1, c, size)];
				//sw += score[countAlignedStoneDir(data, x, y, 0, 0, c, size)];
				sw += score[countAlignedStoneDir(data, x, y, 0, -1, c, size)];
				sw += score[countAlignedStoneDir(data, x, y, -1, 1, c, size)];
				sw += score[countAlignedStoneDir(data, x, y, -1, 0, c, size)];
				sw += score[countAlignedStoneDir(data, x, y, -1, -1, c, size)];
			}
			else if (c == BoardSquare::black)
			{
				sb += score[countAlignedStoneDir(data, x, y, 1, 1, c, size)];
				sb += score[countAlignedStoneDir(data, x, y, 1, 0, c, size)];
				sb += score[countAlignedStoneDir(data, x, y, 1, -1, c, size)];
				sb += score[countAlignedStoneDir(data, x, y, 0, 1, c, size)];
				//sw += score[countAlignedStoneDir(data, x, y, 0, 0, c, size)];
				sb += score[countAlignedStoneDir(data, x, y, 0, -1, c, size)];
				sb += score[countAlignedStoneDir(data, x, y, -1, 1, c, size)];
				sb += score[countAlignedStoneDir(data, x, y, -1, 0, c, size)];
				sb += score[countAlignedStoneDir(data, x, y, -1, -1, c, size)];
			}
		}
	return sw;
}

#include <iostream>

Score AnalyzerBrainDead::getScore(Board& board) {
	Score score;

	score = 0;
	score += countAlignedStone(board, 5);
	score += board.getCapturedBlack() * 5;
	score -= board.getCapturedWhite() * 5;
	return score;
}