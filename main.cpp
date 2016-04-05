
#include <iostream>
#include "GuiManager.hpp"
#include "Game.hpp"
#include <unistd.h>
#include <vector>

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

void game_page(GuiManager& win, bool isBlackAI, bool isWhiteAI)
{
    Game                g(options);
    bool                hasWon = false;
    std::string         text("");
    VictoryState        victory;
    BoardPos            pos;

    while (!hasWon)
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
                        return;
                    break ;
                case sf::Event::MouseButtonPressed:

                    if ((isWhiteAI && isBlackAI) || hasWon)
                    {
                        break;
                    }

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
        }
        win.drawBoard(*g.getState(), text);
        win.display();

        if (g.getTurn() == PlayerColor::blackPlayer && isBlackAI && !hasWon)
        {
            if (g.play())
            {
                hasWon = true;
                text = "Black win\n" + getVictoryMessage(victory);
            }
        }
        win.drawBoard(*g.getState(), text);
        win.display();

        usleep(200);//better framerate system
    }
    usleep(5000000);
}

GuiManager::MenuButton menu_page(GuiManager& win)
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

void option_page(GuiManager& win)
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

int main()
{
    GuiManager          win;

    srand(time(NULL));
    while (1)
    {
        switch (menu_page(win))
        {
            case GuiManager::AIVersusAI:
                game_page(win, true, true);
                break;
            case GuiManager::PlayerVersusAI:
                game_page(win, false, true);
                break;
            case GuiManager::PlayerVersusPlayer:
                game_page(win, false, false);
                break;
            case GuiManager::Options:
                option_page(win);
                break;
        }
    }
}