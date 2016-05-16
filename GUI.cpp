
#include <iostream>
#include "GUIManager.hpp"
#include "Game.hpp"
#include <unistd.h>
#include "GUI.hpp"

using namespace std;

std::string getVictoryMessage(VictoryState v, PlayerColor turn)
{
    std::string text;
    std::string player = (turn == blackPlayer) ? "Black" : "White";
    player += " player wins:\n";

    switch (v)
    {
        case aligned:
            text = player + "  Five stones aligned";
            break;
        case whitesCaptured:
            text = player + "  Enough white stones captured";
            break;
        case blacksCaptured:
            text = player + "  Enough black stones captured";
            break;
        case staleMate:
            text = "  Stalemate";
        default:
            break;
    }
    return (text);
}

void game_page(GUIManager& win, Options &options)
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
                        std::cout << "Player " << (g.getTurn() == whitePlayer ? "white:" : "black:") << std::endl;
                        if (g.play(pos))
                        {
                            hasWon = true;
                            victory = g.getState()->isTerminal(options.capture);
                            text = getVictoryMessage(victory, (PlayerColor)-g.getTurn());
                        }
                        std::cout << "  Board score: " << g.getCurrentScore() << std::endl;
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
                victory = g.getState()->isTerminal(options.capture);
                text = getVictoryMessage(victory, whitePlayer);
            }
            std::cout << "AI white:" << std::endl;
            std::cout << "  Board score: " << g.getCurrentScore() << std::endl;
            std::cout << "  Time taken: " << g.getTimeTaken() << std::endl;
            if (text != "") std::cout << text << std::endl;
            win.drawBoard(g, options, text);
            shouldWait = false;
        }

        if (g.getTurn() == PlayerColor::blackPlayer && options.isBlackAI && !hasWon)
        {
            if (g.play())
            {
                hasWon = true;
                victory = g.getState()->isTerminal(options.capture);
                text = getVictoryMessage(victory, blackPlayer);
            }
            std::cout << "AI black:" << std::endl;
            std::cout << "  Board score: " << g.getCurrentScore() << std::endl;
            std::cout << "  Time taken: " << g.getTimeTaken() << std::endl;
            if (text != "") std::cout << text << std::endl;
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

std::vector<std::pair<std::string, bool>> getOptionsData(Options &options)
{
    return std::vector<std::pair<std::string, bool>>({
                               std::pair<std::string, bool>("Show player tips", options.showTips),
                               std::pair<std::string, bool>("Show square priority", options.showPriority),
                               std::pair<std::string, bool>("Activate deep search", options.slowMode),
                               std::pair<std::string, bool>("Limit black starting moves", options.limitBlack),
                               std::pair<std::string, bool>("Block double free-threes", options.doubleThree),
                               std::pair<std::string, bool>("Allow capture", options.capture),
                               std::pair<std::string, bool>("Allow win by capture", options.captureWin),
                       });
}

void option_page(GUIManager& win, Options &options)
{
    int     pos;
    auto    optionsVector = getOptionsData(options);

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

                    switch (pos)
                    {
                        case 0: options.showTips = !options.showTips; break;
                        case 1: options.showPriority = !options.showPriority; break;
                        case 2: options.slowMode = !options.slowMode; break;
                        case 3: options.limitBlack = !options.limitBlack; break;
                        case 4: options.doubleThree = !options.doubleThree; break;
                        case 5: options.capture = !options.capture; break;
                        case 6: options.captureWin = !options.captureWin; break;
                       // case 7: options.brainDead = !options.brainDead; break;
                    }
                    break;
                default:
                    break;
            }
        }
        optionsVector = getOptionsData(options);
        win.drawOptions(optionsVector);
        win.display();
        usleep(200);
    }
}

void GUI::start_loop()
{
    Options       options;
    GUIManager          win;

    srand(time(NULL));
    while (1)
    {
        switch (menu_page(win))
        {
            case GUIManager::ButtonAIVersusAI:
                options.isBlackAI = true;
                options.isWhiteAI = true;
                game_page(win, options);
                break;
            case GUIManager::ButtonPlayerVersusAI:
                options.isBlackAI = false;
                options.isWhiteAI = true;
                game_page(win, options);
                break;
            case GUIManager::ButtonPlayerVersusPlayer:
                options.isBlackAI = false;
                options.isWhiteAI = false;
                game_page(win, options);
                break;
            case GUIManager::ButtonOptions:
                option_page(win, options);
                break;
        }
    }
}

GUI::GUI(){};
