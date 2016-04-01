//
// Created by Eric DELANGHE on 3/31/16.
//

#pragma once

#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "TextureManager.hpp"

class GuiManager : public sf::RenderWindow {
private:
	static const int screen_width = 1000;
	static const int screen_height = 1000;
	static const int screen_margin_x = 0;
	static const int screen_margin_y = 0;
	static const int board_width = 1000;
	static const int board_height = 1000;
	static const int board_offset_x = 50;
	static const int board_offset_y = 50;
	static const int cell_width = 50;
	static const int cell_height = 50;

	int 					_w;
	int 					_h;
	int 					_lineWidth;
	sf::Color				_colorBG;
	sf::Color				_colorLine;
	SpriteManager			_textures;
public:
	GuiManager();
	~GuiManager();

	bool	getMouseBoardPos(BoardPos& pos);
	void 		drawBoard(const Board& b);
};
