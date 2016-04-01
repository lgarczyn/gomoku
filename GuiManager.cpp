//
// Created by Eric DELANGHE on 3/31/16.
//

#include "GuiManager.hpp"

GuiManager::GuiManager()
		:sf::RenderWindow(sf::VideoMode(screen_width, screen_height), "Gomoku"),
		_w(screen_width),
		_h(screen_height),
		_lineWidth(2),
		_colorBG(173, 216, 230),
		_colorLine(0, 0, 128),
		_textures()
{

}

GuiManager::~GuiManager() { this->close(); }


bool	GuiManager::getMouseBoardPos(BoardPos& pos)
{
	pos = BoardPos(
			(sf::Mouse::getPosition(*this).x - screen_margin_x - board_offset_x + cell_width / 2) / cell_width,
			(sf::Mouse::getPosition(*this).y - screen_margin_y - board_offset_y + cell_width / 2) / cell_height);
	if (pos.x >= 0 && pos.x < BOARD_WIDTH)
		if (pos.y >= 0 && pos.y < BOARD_HEIGHT)
			return true;
	return false;
}

void	GuiManager::drawBoard(const Board &b) {
	BoardSquare 		c;
	sf::Sprite			background(_textures.board);
	sf::Sprite			sprite_black(_textures.stone_black);
	sf::Sprite			sprite_white(_textures.stone_white);
	sf::Sprite			sprite_preview(_textures.stone_preview);
	sf::Sprite			sprite_preview_taboo(_textures.stone_preview_taboo);
	sf::Sprite			sprite_suggestion(_textures.stone_suggestion);

	BoardPos mousePos;
	getMouseBoardPos(mousePos);
	background.setPosition(screen_margin_x, screen_margin_y);
	this->draw(background);

	for (BoardPos pos = BoardPos(); pos != BoardPos::boardEnd; ++pos)
	{
		c = b.getCase(pos);

		sf::Sprite* sprite = nullptr;
		switch (c)
		{
			case BoardSquare::taboo:
				if (pos == mousePos)
					sprite = &sprite_preview_taboo;
				break ;
			case BoardSquare::empty:
				if (pos == mousePos)
					sprite = &sprite_preview;
				break ;
			case BoardSquare::white:
				sprite = &sprite_white;
				break ;
			case BoardSquare::black:
				sprite = &sprite_black;
				break ;
		}
		if (sprite != nullptr)
		{
			sprite->setScale(cell_width / 64., cell_width / 64.);
			sprite->setPosition(
					screen_margin_x + board_offset_x + pos.x * cell_width - cell_width / 2,
					screen_margin_y + board_offset_y + pos.y * cell_height - cell_width / 2);
			this->draw(*sprite);
		}
	}
}