//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#include "BoardPos.hpp"


const BoardPos BoardPos::boardStart = BoardPos(0, 0);
const BoardPos BoardPos::boardEnd = BoardPos(0, BOARD_HEIGHT);

void BoardPos::operator++()
{
	if (x > BOARD_WIDTH - 1)
	{
		x = 0;
		y++;
	}
	else
	{
		x++;
	}
}

bool BoardPos::operator==(const BoardPos& rhs) const
{
	return (x == rhs.x && y == rhs.y);
}

bool BoardPos::operator!=(const BoardPos& rhs) const
{
	return !(rhs == *this);
}