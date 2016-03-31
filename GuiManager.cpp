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

int 	GuiManager::getCellWidth() const { return (_h / BOARD_HEIGHT); }

void	GuiManager::drawBoard(const Board &b) {
	BoardSquare 	c;
	int 			cell_width = getCellWidth();
	int				mx = sf::Mouse::getPosition(*this).x / cell_width;
	int				my = sf::Mouse::getPosition(*this).y / cell_width;
	float 			cell_ratio = 0.1;

	sf::CircleShape		circle(cell_width / 2 - cell_width * cell_ratio);
	sf::RectangleShape	rect(sf::Vector2f(cell_width, cell_width));

	for (int y = 0 ; y < BOARD_HEIGHT; ++y)
	{
		for (int x = 0 ; x < BOARD_WIDTH; ++x)
		{
			c = b.getCase(x, y);
			switch (c)
			{
				case BoardSquare::taboo:
					if (mx == x && my == y)
						circle.setFillColor(sf::Color(255, 120, 40));
					else
						circle.setFillColor(sf::Color(180, 120, 40));
					break ;
				case BoardSquare::empty:
					if (mx == x && my == y)
						circle.setFillColor(sf::Color(180, 120, 255));
					else
						circle.setFillColor(sf::Color(180, 120, 40));
					break ;
				case BoardSquare::white:
					circle.setFillColor(sf::Color(230, 230, 230));
					break ;
				case BoardSquare::black:
					circle.setFillColor(sf::Color(20, 20, 20));
					break ;
			}

			rect.setFillColor(sf::Color(180, 120, 40));
			circle.setPosition(x * cell_width, y * cell_width);
			rect.setPosition(x * cell_width, y * cell_width);
			this->draw(rect);
			this->draw(circle);
		}

	}
}