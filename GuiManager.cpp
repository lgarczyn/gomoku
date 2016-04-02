//
// Created by Eric DELANGHE on 3/31/16.
//

#include "GuiManager.hpp"

#include <sstream>

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


template<class T>
void		centerOnPos(T& target, float x, float y)
{
	sf::FloatRect textRect = target.getLocalBounds();
	target.setOrigin(textRect.left + textRect.width/2.0f,
					 textRect.top  + textRect.height/2.0f);
	target.setPosition(x, y);
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

sf::Vector2f	GuiManager::getMouseScreenRatio()
{
	sf::Vector2i valueInt = sf::Mouse::getPosition(*this);
	sf::Vector2f value = sf::Vector2f(valueInt.x, valueInt.y);
	value.x /= _w;
	value.y /= _h;
	return value;
}

void	GuiManager::drawBoard(const Board &b, std::string message)
{
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
				if (pos == mousePos && !message.size())
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
	if (message.size())
	{
		sf::RectangleShape			wonPopup(sf::Vector2f(_w * 0.8,200));
		sf::Text					wonText(message, _textures.font, 50);

		wonPopup.setFillColor(sf::Color(50, 30, 10, 220));
		wonPopup.setOutlineColor(sf::Color(50, 30, 10, 255));
		wonPopup.setOutlineThickness(4);
		centerOnPos(wonPopup, _w/2, _h/2);
		centerOnPos(wonText, _w/2, _h/2);
		draw(wonPopup);
		draw(wonText);
	}
}


void 		GuiManager::drawMenu()
{
	sf::RectangleShape	lrect(sf::Vector2f(_w, _h / 3));
	sf::RectangleShape	crect(sf::Vector2f(_w, _h / 3));
	sf::RectangleShape	rrect(sf::Vector2f(_w, _h / 3));
	sf::Text			ltext("IA vs IA", _textures.font, 100);
	sf::Text			ctext("IA vs PC", _textures.font, 100);
	sf::Text			rtext("PC vs PC", _textures.font, 100);

	lrect.setFillColor(sf::Color(160, 100, 100));
	crect.setFillColor(sf::Color(100, 160, 100));
	rrect.setFillColor(sf::Color(100, 100, 160));
	lrect.setPosition(0, 0);
	crect.setPosition(0, _h / 3);
	rrect.setPosition(0, _h / 3 * 2);

	centerOnPos(ltext, _w / 2, _h / 3 * 0.5);
	centerOnPos(ctext, _w / 2, _h / 3 * 1.5);
	centerOnPos(rtext, _w / 2, _h / 3 * 2.5);

	this->draw(lrect);
	this->draw(crect);
	this->draw(rrect);
	this->draw(ltext);
	this->draw(ctext);
	this->draw(rtext);
}