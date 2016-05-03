//
// Created by Eric DELANGHE on 3/31/16.
//

#pragma once

#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "TextureManager.hpp"
#include "Options.hpp"

class Game;

class GUIManager : public sf::RenderWindow {
public:

	enum MenuButton
	{
		ButtonPlayerVersusAI = 0,
		ButtonPlayerVersusPlayer = 1,
		ButtonAIVersusAI = 2,
		ButtonOptions = 3,
	};

	GUIManager();
	~GUIManager();

	MenuButton		getMenuButton();
	bool			getMouseBoardPos(BoardPos& pos);
	sf::Vector2f	getMouseScreenRatio();
	void			drawBoard(Game& g, Options options, const std::string message);
	void 			drawOptions(std::vector<std::pair<std::string, bool>> options);
	void 			drawMenu();
private:
	static const int screen_width = 1140;
	static const int screen_height = 1000;
	static const int screen_margin_x = 70;
	static const int screen_margin_y = 0;
	static const int board_width = 1000;
	static const int board_height = 1000;
	static const int board_offset_x = 50;
	static const int board_offset_y = 50;
	static const int cell_width = 50;
	static const int cell_height = 50;

	static const int score_cell_count = 18;
	static const int score_cell_height = screen_height / score_cell_count;
	static const int score_offset_x = screen_margin_x / 2;
	static const int score_offset_y = score_cell_height / 2;

	int 					_w;
	int 					_h;
	int 					_lineWidth;
	sf::Color				_colorBG;
	sf::Color				_colorLine;
	SpriteManager			_textures;
};
