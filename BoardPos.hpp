//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#pragma once

#include "Constants.hpp"

//Declaration here for optimizaton purpose

class BoarPos;

class BoardPos
{
public:
	int x;
	int y;
	BoardPos(int _x, int _y): x(_x), y(_y) {};
	BoardPos(): x(0), y(0) {};

	static const BoardPos boardEnd;

	void operator++()
	{
		if (x >= BOARD_WIDTH - 1)
		{
			x = 0;
			y++;
		}
		else
		{
			x++;
		}
	}

	bool operator==(const BoardPos& rhs) const
	{
		return (x == rhs.x && y == rhs.y);
	}

	bool operator!=(const BoardPos& rhs) const
	{
		return !(rhs == *this);
	}
};

