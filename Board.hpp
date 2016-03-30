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
		BoardPos(int _x, int _y): x(_x), y(_y) {};
		BoardPos(): x(0), y(0) {};
	};

	bool isTerminal();
	Board():_captured(){};
private:
	BoardSquare _data[19][19]();
	int _captured;
};


