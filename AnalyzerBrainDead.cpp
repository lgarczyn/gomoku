//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "AnalyzerBrainDead.hpp"

AnalyzerBrainDead::AnalyzerBrainDead() { }

Score AnalyzerBrainDead::getScore(Board& board) {
	Score score;

	score = 0;
	for (int y = 0 ; y < BOARD_HEIGHT; ++y)
		for (int x = 0 ; x < BOARD_WIDTH; ++x)
			for (int dirX = -1 ; dirX <= 1; ++dirX)
				for (int dirY = -1 ; dirY <= 1; ++dirY)
					if (board.isAlignedStoneDir(x, y, dirX, dirY, (*board.getData())[y][x], 5)) {
						if ((*board.getData())[y][x] == BoardSquare::white)
							++score;
						else
							--score;
					}
	return score;
}