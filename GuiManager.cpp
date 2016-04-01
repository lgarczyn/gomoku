//
// Created by Eric DELANGHE on 3/31/16.
//

#include "GuiManager.hpp"

GuiManager::GuiManager(int width, int height, const char* title)
		:
		sf::RenderWindow(sf::VideoMode(width, height), title),
		_w(width),
		_h(height),
		_lineWidth(2),
		_colorBG(173, 216, 230),
		_colorLine(0, 0, 128),
		_textures()
{

}

GuiManager::~GuiManager() { this->close(); }

int 	GuiManager::getCellWidth() const { return (_h / BOARD_HEIGHT); }

void	GuiManager::drawBoard(const Board &b) {
	BoardSquare 		c;
	int 				cell_width = getCellWidth();
	int					offset = (_w % cell_width) / 2;
	sf::Sprite			sprite_black(_textures.stone_black);
	sf::Sprite			sprite_white(_textures.stone_white);
	sf::Sprite			sprite_preview(_textures.stone_preview);
	sf::Sprite			sprite_preview_taboo(_textures.stone_preview_taboo);
	sf::Sprite			sprite_suggestion(_textures.stone_suggestion);

	sf::RectangleShape	rect;
	BoardPos			mousePos = BoardPos(
			sf::Mouse::getPosition(*this).x / cell_width,
			sf::Mouse::getPosition(*this).y / cell_width);

	this->clear(_colorBG);
	rect = sf::RectangleShape(sf::Vector2<float>(_lineWidth, _h));
	rect.setFillColor(_colorLine);
	for (int x = 0; x < BOARD_WIDTH + 1; x++)
	{
		rect.setPosition(offset + x * cell_width + cell_width / 2 - _lineWidth / 2, 0);
		this->draw(rect);
	}
	rect = sf::RectangleShape(sf::Vector2<float>(_w, _lineWidth));
	rect.setFillColor(_colorLine);
	for (int y = 0; y < BOARD_HEIGHT + 1; y++)
	{
		rect.setPosition(0, offset + y * cell_width + cell_width / 2 - _lineWidth / 2);
		this->draw(rect);
	}

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
					offset + pos.x * cell_width,
					offset + pos.y * cell_width);
			this->draw(*sprite);
		}
	}
}