//
// Created by Eric DELANGHE on 3/31/16.
//

#include "GUIManager.hpp"

#include <sstream>
#include "Game.hpp"

GUIManager::GUIManager()
		:sf::RenderWindow(sf::VideoMode(screen_width, screen_height), "Gomoku", sf::Style::Titlebar | sf::Style::Close),
		_w(screen_width),
		_h(screen_height),
		_colorBG(173, 216, 230),
		_colorLine(0, 0, 128)
{

}

SpriteManager GUIManager::_textures = SpriteManager();

template<class T>
void		centerOnPos(T& target, float x, float y)
{
	sf::FloatRect textRect = target.getLocalBounds();
	target.setOrigin(textRect.left + textRect.width/2.0f,
					 textRect.top  + textRect.height/2.0f);
	target.setPosition(x, y);
}

GUIManager::~GUIManager() { this->close(); }

GUIManager::MenuButton		GUIManager::getMenuButton()
{
	auto mousePos = getMouseScreenRatio();

	int x = mousePos.x * 2;
	int y = mousePos.y * 2;

	return (static_cast<MenuButton >(y * 2 + x));
}

bool	GUIManager::getMouseBoardPos(BoardPos& pos)
{
	pos = BoardPos(
			(sf::Mouse::getPosition(*this).x - screen_margin_x - board_offset_x + cell_width / 2) / cell_width,
			(sf::Mouse::getPosition(*this).y - screen_margin_y - board_offset_y + cell_width / 2) / cell_height);
	if (pos.x >= 0 && pos.x < BOARD_WIDTH)
		if (pos.y >= 0 && pos.y < BOARD_HEIGHT)
			return true;
	return false;
}

sf::Vector2f	GUIManager::getMouseScreenRatio()
{
	sf::Vector2i valueInt = sf::Mouse::getPosition(*this);
	sf::Vector2f value = sf::Vector2f(valueInt.x, valueInt.y);
	value.x /= _w;
	value.y /= _h;
	return value;
}

void	GUIManager::drawBoard(Game& g, Options options, const std::string message, BoardPos *bestMove)
{
	BoardSquare 		c;
	sf::Sprite			highlight(_textures.highlight);
	sf::Sprite			background(_textures.board);
	sf::Sprite			sprite_black(_textures.stone_black);
	sf::Sprite			sprite_white(_textures.stone_white);
	sf::Sprite			sprite_preview_play(_textures.stone_preview_taboo);
	sf::Sprite			sprite_preview(_textures.stone_preview);
	sf::Sprite			sprite_preview_taboo(_textures.stone_preview_taboo);
	sf::Sprite			sprite_preview_taboo_mouse(_textures.stone_preview_taboo);
	sf::Sprite			sprite_suggestion(_textures.stone_suggestion);
	sf::Text			text_victory;

	Board &b = *g.getState();
	bool isPlayerNext = g.isPlayerNext();

	BoardPos mousePos;
	getMouseBoardPos(mousePos);

	background.setPosition(0, 0);

	if (b.isFlaggedFinal())
		background.setColor(sf::Color(123, 255, 255));
	else
		background.setColor(sf::Color(255, 255, 255));

	this->draw(background);

	MoveScore bestPriority = b.getBestPriority();

	for (BoardPos pos = BoardPos(); pos != BoardPos::boardEnd; ++pos)
	{
		c = b.getCase(pos);
		int p = b.getPriority(pos);
		bool isHighlighted = g.hasPosChanged(pos);

		//check if piece of priority text is needed
		//Jesus Eric you comments make no sense in hell
		sf::Sprite* sprite = nullptr;
		sf::Text* text = nullptr;
		switch (c)
		{
			case BoardSquare::empty:
				if (p == -1)
				{
					if (pos == mousePos && isPlayerNext)
						sprite = &sprite_preview_taboo_mouse;
					else
						sprite = &sprite_preview_taboo;
				}
				else
				{
					if (p > 0)
					{
						text_victory = sf::Text(std::to_string(p), _textures.font, 20);
						text = &text_victory;
						text->setFillColor(sf::Color(0, 0, 0));
					}
					if (bestMove && *bestMove == pos)
						sprite = &sprite_suggestion;
					else if (pos == mousePos && !message.size() && isPlayerNext)
						sprite = &sprite_preview;
					else if (isPlayerNext && p == bestPriority.score && options.showTips) // pos == bestPriority)
					{
						sprite = &sprite_preview;
					}

				}
				break ;
			case BoardSquare::white:
				sprite = &sprite_white;
				break ;
			case BoardSquare::black:
				sprite = &sprite_black;
				break ;
		}
		//draw highlight
		if (isHighlighted)
		{
			highlight.setScale(cell_width / 100., cell_width / 100.);
			centerOnPos(highlight,
						screen_margin_x + board_offset_x + pos.x * cell_width,
						screen_margin_y + board_offset_y + pos.y * cell_height);
			this->draw(highlight);
		}
		//draw piece
		if (sprite != nullptr)
		{
			sprite->setScale(cell_width / 64., cell_width / 64.);
			centerOnPos(*sprite,
						screen_margin_x + board_offset_x + pos.x * cell_width,
						screen_margin_y + board_offset_y + pos.y * cell_height);
			this->draw(*sprite);
		}
		//draw priority
		if (text != nullptr && options.showPriority)
		{
			text->setFillColor(sf::Color::White);
			sf::Vector2f textPos = sf::Vector2f(
					screen_margin_x + board_offset_x + pos.x * cell_width,
					screen_margin_y + board_offset_y + pos.y * cell_height);

			sf::Vector2f size = sf::Vector2f(40, 20);
			sf::RectangleShape shape = sf::RectangleShape(size);
			shape.setFillColor(sf::Color(50, 30, 10, 220));
			shape.setOutlineColor(sf::Color(50, 30, 10, 255));
			shape.setOutlineThickness(1);
			shape.setPosition(textPos.x - 20, textPos.y - 10);

			centerOnPos(*text, textPos.x, textPos.y);
			this->draw(shape);
			this->draw(*text);
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

	sprite_black.setScale(cell_width / 64., cell_width / 64.);
	int count = b.getCapturedBlack();
	if (count > score_cell_count)
		count = score_cell_count;
	for (int i = 0; i < count; i++)
	{
		int posx = score_offset_x;
		int posy = (screen_height * (i + 0.5) / score_cell_count);
		centerOnPos(sprite_black, posx, posy);
		draw(sprite_black);
	}

	sprite_white.setScale(cell_width / 64., cell_width / 64.);
	count = b.getCapturedWhite();
	if (count > score_cell_count)
		count = score_cell_count;
	for (int i = 0; i < count; i++)
	{
		int posx = screen_width - score_offset_x;
		int posy = (screen_height * (i + 0.5) / score_cell_count);
		centerOnPos(sprite_white, posx, posy);
		draw(sprite_white);
	}

	display();
}

void 		GUIManager::drawMenu()
{
	sf::RectangleShape	ulrect(sf::Vector2f(_w / 2, _h / 2));
	sf::RectangleShape	urrect(sf::Vector2f(_w / 2, _h / 2));
	sf::RectangleShape	llrect(sf::Vector2f(_w / 2, _h / 2));
	sf::RectangleShape	lrrect(sf::Vector2f(_w / 2, _h / 2));


	sf::Text			ultext("PC vs IA", _textures.font, 100);
	sf::Text			urtext("PC vs PC", _textures.font, 100);
	sf::Text			lltext("AI vs AI", _textures.font, 100);
	sf::Text			lrtext("Settings", _textures.font, 100);

	ulrect.setFillColor(sf::Color(160, 100, 100));
	urrect.setFillColor(sf::Color(100, 160, 100));
	llrect.setFillColor(sf::Color(100, 100, 160));
	lrrect.setFillColor(sf::Color(120, 120, 120));
	ulrect.setPosition(0, 0);
	urrect.setPosition(_w / 2, 0);
	llrect.setPosition(0, _h / 2);
	lrrect.setPosition(_w / 2, _h / 2);

	centerOnPos(ultext, _w / 4 * 1, _h / 4 * 1);
	centerOnPos(urtext, _w / 4 * 3, _h / 4 * 1);
	centerOnPos(lltext, _w / 4 * 1, _h / 4 * 3);
	centerOnPos(lrtext, _w / 4 * 3, _h / 4 * 3);

	this->draw(ulrect);
	this->draw(urrect);
	this->draw(llrect);
	this->draw(lrrect);
	this->draw(ultext);
	this->draw(urtext);
	this->draw(lltext);
	this->draw(lrtext);
}

void 		GUIManager::drawOptions(std::vector<std::pair<std::string, bool>> options)
{
	sf::RectangleShape	rect(sf::Vector2f(_w, _h / options.size()));
	sf::Text			text;

	int i = 0;
	for (auto pair:options)
	{
		if (pair.second)
			rect.setFillColor(sf::Color(100, 160, 100));
		else
			rect.setFillColor(sf::Color(160, 100, 100));
		text = sf::Text(pair.first, _textures.font, 70);

		auto pos = sf::Vector2f(_w / 2., (float)_h / options.size() * (i + 0.5));
		centerOnPos(rect, pos.x, pos.y);
		centerOnPos(text, pos.x, pos.y);
		this->draw(rect);
		this->draw(text);
		i++;
	}
}