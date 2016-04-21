
#include <iostream>
#include "GUIManager.hpp"
#include "Game.hpp"
#include <unistd.h>
#include "GUI.hpp"

using namespace std;

Game::Options options;

std::string getVictoryMessage(VictoryState v)
{
    std::string text;

    switch (v)
    {
        case aligned:
            text = "Five stone aligned";
            break;
        case whitesCaptured:
            text = "Enough whites stone captured";
            break;
        case blacksCaptured:
            text = "Enough blacks stone captured";
            break;
        default:
            break;
    }
    return (text);
}

void game_page(GUIManager& win, bool isBlackAI, bool isWhiteAI)
{
    Game                g(options);
    bool                hasWon = false;
    std::string         text("");
    VictoryState        victory;
    BoardPos            pos;
    BoardPos            suggestion;


    while (1)
    {
        bool isPlayerNext;

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
                        return;
                    break ;
                case sf::Event::MouseButtonPressed:

                    if (hasWon)
                        return;

                    if (win.getMouseBoardPos(pos) && !hasWon)
                    {
                        if (g.play(pos))
                        {
                            hasWon = true;
                            text = "Victory\n" + getVictoryMessage(victory);
                        }
                    }

                    break ;
                default:
                    break;
            }
        }

        if (g.getTurn() == PlayerColor::whitePlayer && isWhiteAI && !hasWon)
        {
            if (g.play())
            {
                hasWon = true;
                text = "White win\n" + getVictoryMessage(victory);
            }
            else if (!isBlackAI)
            {
                suggestion = g.getNextMove();
            }
        }
        isPlayerNext = (!isBlackAI && g.getTurn() == blackPlayer) || (!isWhiteAI && g.getTurn() == whitePlayer);
        win.drawBoard(*g.getState(), isPlayerNext, text);
        win.display();

        if (g.getTurn() == PlayerColor::blackPlayer && isBlackAI && !hasWon)
        {
            if (g.play())
            {
                hasWon = true;
                text = "Black win\n" + getVictoryMessage(victory);
            }
            else if (!isWhiteAI)
            {
                suggestion = g.getNextMove();
            }
        }
        isPlayerNext = (!isBlackAI && g.getTurn() == blackPlayer) || (!isWhiteAI && g.getTurn() == whitePlayer);
        win.drawBoard(*g.getState(), isPlayerNext, text);
        win.display();

        if (isPlayerNext)
            usleep(200);
    }
}

GUIManager::MenuButton menu_page(GUIManager& win)
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

std::vector<std::pair<std::string, bool>> getOptionsData()
{
    return std::vector<std::pair<std::string, bool>>({
                               std::pair<std::string, bool>("Limit black starting moves", options.limitBlack),
                               std::pair<std::string, bool>("Block double free-threes", options.doubleThree),
                               std::pair<std::string, bool>("Allow capture", options.capture),
                               std::pair<std::string, bool>("Allow win by capture", options.captureWin),
                               std::pair<std::string, bool>("Use neural network", !options.brainDead)
                       });
}

void option_page(GUIManager& win)
{
    int pos;
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
                    pos = win.getMouseScreenRatio().y * 5;

                    switch (pos)
                    {
                        case 0: options.limitBlack = !options.limitBlack; break;
                        case 1: options.doubleThree = !options.doubleThree; break;
                        case 2: options.capture = !options.capture; break;
                        case 3: options.captureWin = !options.captureWin; break;
                        case 4: options.brainDead = !options.brainDead; break;
                    }
                    break;
                default:
                    break;
            }
        }
        win.drawOptions(getOptionsData());
        win.display();
        usleep(200);
    }
}

void GUI::start_loop()
{
    GUIManager          win;

    srand(time(NULL));
    while (1)
    {
        switch (menu_page(win))
        {
            case GUIManager::AIVersusAI:
                game_page(win, true, true);
                break;
            case GUIManager::PlayerVersusAI:
                game_page(win, false, true);
                break;
            case GUIManager::PlayerVersusPlayer:
                game_page(win, false, false);
                break;
            case GUIManager::Options:
                option_page(win);
                break;
        }
    }
}

GUI::GUI(){};