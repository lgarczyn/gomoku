//
// Created by Louis GARCZYNSKI on 5/3/16.
//


#pragma once

#include <utility>
#include <vector>

class Options
{
public:
	bool showTips = true;
	bool showSolution = false;
	bool showTipsDebug = false;
	bool limitBlack = false;
	bool doubleThree = true;
	bool capture = true;
	bool captureWin = true;
	bool isBlackAI = true;
	bool isWhiteAI = true;
	bool slowMode = false;

	std::vector<std::pair<const char *, bool&>> as_array() {
		return std::vector<std::pair<const char *, bool&>>({
			std::pair<const char *, bool&>("Show player tips", showTips),
			std::pair<const char *, bool&>("Show player tips debug", showTipsDebug),
			std::pair<const char *, bool&>("Show player solution", showSolution),
			std::pair<const char *, bool&>("Activate deep search", slowMode),
			std::pair<const char *, bool&>("Limit black starting moves", limitBlack),
			std::pair<const char *, bool&>("Block double free-threes", doubleThree),
			std::pair<const char *, bool&>("Allow capture", capture),
			std::pair<const char *, bool&>("Allow win by capture", captureWin),
		});
	}
};


