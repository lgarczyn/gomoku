//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#pragma once

#include "Constants.hpp"

//Declaration here for optimizaton purpose

struct BoardPos
{
	int x;
	int y;
	constexpr BoardPos(int _x, int _y): x(_x), y(_y) {};
	constexpr BoardPos(): x(0), y(0) {};

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

    constexpr bool operator==(const BoardPos& rhs) const
	{
		return (x == rhs.x && y == rhs.y);
	}

    constexpr bool operator!=(const BoardPos& rhs) const
	{
		return !(rhs == *this);
	}

    static constexpr BoardPos boardEnd(){
        return BoardPos(0, BOARD_HEIGHT);
    }
};