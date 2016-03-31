//
// Created by Eric DELANGHE on 3/31/16.
//

#ifndef GOMOKU_GUIMANAGER_HPP
#define GOMOKU_GUIMANAGER_HPP

#include <SFML/Graphics.hpp>
#include "Board.hpp"

class GuiManager : public sf::RenderWindow {
private:
	int 					_w;
	int 					_h;
public:
	GuiManager(int width, int height, const char* title);
	~GuiManager();

	void 	drawBoard(const Board& b);
	int 	getCellWidth() const;
};


#endif //GOMOKU_GUIMANAGER_HPP
