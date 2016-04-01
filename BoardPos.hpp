//
// Created by Louis GARCZYNSKI on 3/30/16.
//

#pragma once

#include "Constants.hpp"

class BoardPos
{
public:
	int x;
	int y;
	BoardPos(int _x, int _y): x(_x), y(_y) {};
	BoardPos(): x(0), y(0) {};

	static const BoardPos boardStart;
	static const BoardPos boardEnd;



	void operator++();
	bool operator==(const BoardPos& rhs) const;
	bool operator!=(const BoardPos& rhs) const;
};


