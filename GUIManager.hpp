//
// Created by Eric DELANGHE on 3/31/16.
//

#pragma once

#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "TextureManager.hpp"

class GUIManager : public sf::RenderWindow {
public:

	enum MenuButton
	{
		PlayerVersusAI = 0,
		PlayerVersusPlayer = 1,
		AIVersusAI = 2,
		Options = 3,
	};

	GUIManager();
	~GUIManager();

	MenuButton		getMenuButton();
	bool			getMouseBoardPos(BoardPos& pos);
	sf::Vector2f	getMouseScreenRatio();
	void 			drawBoard(const Board& b, std::string text);
	void 			drawOptions(std::vector<std::pair<std::string, bool>> options);
	void 			drawMenu();
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
};
