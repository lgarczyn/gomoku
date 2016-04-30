//
// Created by Louis GARCZYNSKI on 4/1/16.
//


#pragma once

#include <limits>

using Score=int;
#define BOARD_WIDTH 19
#define BOARD_HEIGHT 19
const Score pinfinity = std::numeric_limits<Score>::max() / 2;
const Score ninfinity = std::numeric_limits<Score>::min() / 2;
const int captureVictoryPoints = 10;
