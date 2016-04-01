//
// Created by Louis GARCZYNSKI on 4/1/16.
//


#pragma once

#include <SFML/Graphics.hpp>

struct SpriteManager
{
	sf::Texture				board;
	sf::Texture 			stone_black;
	sf::Texture 			stone_white;
	sf::Texture 			stone_suggestion;
	sf::Texture 			stone_preview;
	sf::Texture 			stone_preview_taboo;
	sf::Font				font;

	SpriteManager();
};


