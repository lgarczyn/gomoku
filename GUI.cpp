
#include <iostream>
#include "GUIManager.hpp"
#include "Game.hpp"
#include <unistd.h>
#include "GUI.hpp"

using namespace std;

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

void game_page(GUIManager& win, Game::Options &options)
{
    Game                g(options);
    bool                hasWon = false;
    std::string         text("");
    VictoryState        victory;
    BoardPos            pos;

    win.drawBoard(g, options, text);
    while (1)
    {
        bool shouldWait = true;
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

                    if (g.isPlayerNext() && win.getMouseBoardPos(pos) && !hasWon)
                    {
                        if (g.play(pos))
                        {
                            hasWon = true;
                            text = "Victory\n" + getVictoryMessage(victory);
                        }
                        win.drawBoard(g, options, text);
                    }
                    break ;
                default:
                    break;
            }
        }

        if (g.isPlayerNext())
            win.drawBoard(g, options, text);

        if (g.getTurn() == PlayerColor::whitePlayer && options.isWhiteAI && !hasWon)
        {
            if (g.play())
            {
                hasWon = true;
                text = "White win\n" + getVictoryMessage(victory);
            }
            win.drawBoard(g, options, text);
            shouldWait = false;
        }

        if (g.getTurn() == PlayerColor::blackPlayer && options.isBlackAI && !hasWon)
        {
            if (g.play())
            {
                hasWon = true;
                text = "Black win\n" + getVictoryMessage(victory);
            }
            win.drawBoard(g, options, text);
            shouldWait = false;
        }

        if (shouldWait)
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

std::vector<std::pair<std::string, bool>> getOptionsData(Game::Options &options)
{
    return std::vector<std::pair<std::string, bool>>({
                               std::pair<std::string, bool>("Show player tips", options.showTips),
                               std::pair<std::string, bool>("Show square priority", options.showPriority),
                               std::pair<std::string, bool>("Limit black starting moves", options.limitBlack),
                               std::pair<std::string, bool>("Block double free-threes", options.doubleThree),
                               std::pair<std::string, bool>("Allow capture", options.capture),
                               std::pair<std::string, bool>("Allow win by capture", options.captureWin),
                               std::pair<std::string, bool>("Use neural network", !options.brainDead)
                       });
}

void option_page(GUIManager& win, Game::Options &options)
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
                    pos = win.getMouseScreenRatio().y * 7;

                    switch (pos)
                    {
                        case 0: options.showTips = !options.showTips; break;
                        case 1: options.showPriority = !options.showPriority; break;
                        case 2: options.limitBlack = !options.limitBlack; break;
                        case 3: options.doubleThree = !options.doubleThree; break;
                        case 4: options.capture = !options.capture; break;
                        case 5: options.captureWin = !options.captureWin; break;
                        case 6: options.brainDead = !options.brainDead; break;
                    }
                    break;
                default:
                    break;
            }
        }
        win.drawOptions(getOptionsData(options));
        win.display();
        usleep(200);
    }
}

void GUI::start_loop()
{
    Game::Options       options;
    GUIManager          win;

    srand(time(NULL));
    while (1)
    {
        switch (menu_page(win))
        {
            case GUIManager::AIVersusAI:
                options.isBlackAI = true;
                options.isWhiteAI = true;
                game_page(win, options);
                break;
            case GUIManager::PlayerVersusAI:
                options.isBlackAI = false;
                options.isWhiteAI = true;
                game_page(win, options);
                break;
            case GUIManager::PlayerVersusPlayer:
                options.isBlackAI = false;
                options.isWhiteAI = true;
                game_page(win, options);
                break;
            case GUIManager::Options:
                option_page(win, options);
                break;
        }
    }
}

GUI::GUI(){};