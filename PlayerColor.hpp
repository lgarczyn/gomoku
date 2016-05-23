//
// Created by Louis GARCZYNSKI on 3/31/16.
//


#pragma once

enum PlayerColor
{
	blackPlayer = -1,
	nullPlayer  = 0,
	whitePlayer = 1,
};

PlayerColor operator-(PlayerColor& rhs);


