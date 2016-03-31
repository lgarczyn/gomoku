//
// Created by Eric DELANGHE on 3/31/16.
//

#include "GuiManager.hpp"

GuiManager::GuiManager(int width, int height, const char* title)
		:
		sf::RenderWindow(sf::VideoMode(width, height), title),
		_w(width),
		_h(height)
{


}

GuiManager::~GuiManager() { this->close(); }

void	GuiManager::draw_board(const Board &b) {
	BoardSquare 	c;
	int 			cell_width = _h / BOARD_HEIGHT;

	sf::RectangleShape rect(sf::Vector2f(0, 0));
	rect.setSize(sf::Vector2f(cell_width, cell_width));

	for (int x = 0 ; x < BOARD_WIDTH; ++x)
	{
		for (int y = 0 ; y < BOARD_HEIGHT; ++y)
		{
			c = b.getCase(x, y);
			switch (c)
			{
				case BoardSquare::taboo:
				case BoardSquare::empty:
					rect.setFillColor(sf::Color(180, 120, 40));
					break ;
				case BoardSquare::white:
					rect.setFillColor(sf::Color(230, 230, 230));
					break ;
				case BoardSquare::black:
					rect.setFillColor(sf::Color(20, 20, 20));
					break ;
			}
			rect.setPosition(x * cell_width, y * cell_width);
			this->draw(rect);
		}

	}
}