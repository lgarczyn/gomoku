
#include <iostream>
#include "GUIManager.hpp"
#include "Game.hpp"
#include <unistd.h>
#include "GUI.hpp"

using namespace std;

std::string getVictoryMessage(VictoryState v)
{
	std::string text;
	std::string player = (v.victor == blackPlayer) ? "Black" : "White";
	player += " player wins:\n";

	switch (v.type)
	{
		case aligned:
			text = player + "  Five stones aligned";
			break;
		case captured:
			text = player + "  Enough stones captured";
			break;
		case staleMate:
			text = "Stalemate";
		default:
			break;
	}
	return (text);
}

void gamePage(GUIManager& win, Options &options)
{
	Game g(options);
	bool hasWon = false;
	std::string text("");
	VictoryState victory;
	bool updateSuggestion = true;
	BoardPos suggestion;

	win.drawBoard(g, options, text, NULL);
	while (1)
	{
		bool shouldWait = true;
		win.clear();
		sf::Event   event;
		BoardPos mousePos;
		
		while (win.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					exit(0);
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Escape)
						return;
					break ;
				case sf::Event::MouseButtonPressed:
					if (hasWon)
						return;
					if (g.isPlayerNext() && win.getMouseBoardPos(mousePos) && !hasWon)
					{
						std::cout << "Player " << (g.getTurn() == whitePlayer ? "white:" : "black:") << std::endl;
						if (g.play(mousePos))
						{
							hasWon = true;
							victory = g.getState()->getVictory();
							text = getVictoryMessage(victory);
						}
						std::cout << "  Board score: " << g.getCurrentScore() << std::endl;
						win.drawBoard(g, options, text, NULL);
						updateSuggestion = true;
					}
					break ;
				default:
					break;
			}
		}

		if (g.isPlayerNext())
		{
			if (updateSuggestion && options.showSolution)
			{
				updateSuggestion = false;
				suggestion = g.getNextMove();
			}
			win.drawBoard(g, options, text, options.showSolution ? &suggestion : NULL);
		}

		if (g.getTurn() == PlayerColor::whitePlayer && options.isWhiteAI && !hasWon)
		{
			std::cout << "AI white:" << std::endl;
			if (g.play())
			{
				hasWon = true;
				victory = g.getState()->getVictory();
				text = getVictoryMessage(victory);
				std::cout << text << std::endl;
			}
			std::cout << "  Board score: " << g.getCurrentScore() << std::endl;
			std::cout << "  Time taken: " << g.getTimeTaken() << std::endl;
			std::cout << "  Move explored: " << g.getMovesExplored() << std::endl;
			
			win.drawBoard(g, options, text, NULL);
			shouldWait = false;
			updateSuggestion = true;
		}

		if (g.getTurn() == PlayerColor::blackPlayer && options.isBlackAI && !hasWon)
		{
			std::cout << "AI black:" << std::endl;

			if (g.play())
			{
				hasWon = true;
				victory = g.getState()->getVictory();
				text = getVictoryMessage(victory);
				std::cout << text << std::endl;
			}
			std::cout << "  Board score: " << g.getCurrentScore() << std::endl;
			std::cout << "  Time taken: " << g.getTimeTaken() << std::endl;
			std::cout << "  Move explored: " << g.getMovesExplored() << std::endl;
			
			win.drawBoard(g, options, text, NULL);
			shouldWait = false;
			updateSuggestion = true;
		}

		if (shouldWait)
			usleep(200);
	}
}

GUIManager::MenuButton menuPage(GUIManager& win)
{
	while (1)
	{
		win.clear();
		sf::Event   event;
		while (win.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					exit(0);
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Escape)
						exit(0);
					break ;
				case sf::Event::MouseButtonPressed:
					return win.getMenuButton();
				default:
					break;
			}
		}
		win.drawMenu();
		win.display();
		usleep(200);
	}
}

void optionPage(GUIManager& win, Options &options)
{
	int     pos;
	auto    optionsVector = options.as_array();

	while (1)
	{
		win.clear();
		sf::Event   event;
		while (win.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					exit(0);
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Escape)
						return ;
					break ;
				case sf::Event::MouseButtonPressed:
					pos = win.getMouseScreenRatio().y * optionsVector.size();

					if (pos < optionsVector.size())
						optionsVector[pos].second = !optionsVector[pos].second;
					break;
				default:
					break;
			}
		}
		win.drawOptions(options);
		win.display();
		usleep(200);
	}
}

void GUI::startLoop()
{
	Options       options;
	GUIManager          win;

	srand(time(NULL));
	while (1)
	{
		switch (menuPage(win))
		{
			case GUIManager::ButtonAIVersusAI:
				options.isBlackAI = true;
				options.isWhiteAI = true;
				gamePage(win, options);
				break;
			case GUIManager::ButtonPlayerVersusAI:
				options.isBlackAI = false;
				options.isWhiteAI = true;
				gamePage(win, options);
				break;
			case GUIManager::ButtonPlayerVersusPlayer:
				options.isBlackAI = false;
				options.isWhiteAI = false;
				gamePage(win, options);
				break;
			case GUIManager::ButtonOptions:
				optionPage(win, options);
				break;
		}
	}
}

GUI::GUI(){};
