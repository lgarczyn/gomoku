//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#pragma once


class Board
{
public:
	enum BoardSquare
	{
		empty = 0,
		white = 1,
		black = 2,
		taboo = 3
	};

	class BoardPos
	{
	public:
		int x;
		int y;
		BoardPos(int _x, int _y);
		BoardPos();
	};

	bool isTerminal();
	Board(void);
private:
	BoardSquare _data[19][19];
	int _captured;
};


