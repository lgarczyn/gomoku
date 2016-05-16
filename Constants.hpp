//
// Created by Louis GARCZYNSKI on 4/1/16.
//


#pragma once

#include <limits>

using Score = long long int;
#define BOARD_WIDTH 19
#define BOARD_HEIGHT 19
const Score pinfinity = std::numeric_limits<Score>::max() / 4;
const Score ninfinity = -pinfinity;

const int captureVictoryPoints = 10;
const int capturePriority = 30;

#define CLAMP(x, min, max) ((x < min) ? min : ((x > max) ? max : x))