//
// Created by Louis GARCZYNSKI on 4/1/16.
//


#pragma once

#include <limits>

using Score=int;
#define BOARD_WIDTH 19
#define BOARD_HEIGHT 19
const Score pinfinity = std::numeric_limits<Score>::max() / 2;
const Score ninfinity = -pinfinity;

const bool slowMode = true;
const double time_linit = (slowMode ? 3 : 0.5);
const double time_margin = 0.001;
const int captureVictoryPoints = 10;
