//
// Created by Louis GARCZYNSKI on 4/1/16.
//

#include "TextureManager.hpp"


SpriteManager::SpriteManager()
{
	if (!stone_black.loadFromFile("./textures/stone_black.png"))
	{
		throw std::logic_error("Could not load black stone texture");
	}
	if (!stone_white.loadFromFile("./textures/stone_white.png"))
	{
		throw std::logic_error("Could not load white stone texture");
	}
	if (!stone_suggestion.loadFromFile("./textures/stone_suggestion.png"))
	{
		throw std::logic_error("Could not load suggestion stone texture");
	}
	if (!stone_preview.loadFromFile("./textures/stone_preview.png"))
	{
		throw std::logic_error("Could not load preview stone texture");
	}
	if (!stone_preview_taboo.loadFromFile("./textures/stone_preview_taboo.png"))
	{
		throw std::logic_error("Could not load preview taboo stone texture");
	}
	stone_black.setSmooth(true);
	stone_white.setSmooth(true);
	stone_suggestion.setSmooth(true);
	stone_preview.setSmooth(true);
	stone_preview_taboo.setSmooth(true);
}