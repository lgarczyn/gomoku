
#include <iostream>
#include "GuiManager.hpp"
#include "Game.hpp"
#include <unistd.h>

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
    }
    return (text);
}

void game_page(GuiManager& win, bool isBlackAI, bool isWhiteAI)
{
    Game                g(true);
    bool                hasWon = false;
    std::string         text("");
    VictoryState        victory;

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
                    BoardPos pos;

                    if ((isWhiteAI && isBlackAI) || hasWon)
                    {
                        break;
                    }

                    if (win.getMouseBoardPos(pos) && !hasWon)
                    {
                        g.play(pos);
                    }

                    if ((victory = g.getState()->isTerminal()) && !hasWon)
                    {
                        hasWon = true;
                        text = "Victory\n" + getVictoryMessage(victory);
                    }

                    break ;
            }
        }

        if (g.getTurn() == PlayerColor::whitePlayer && isWhiteAI && !hasWon)
        {
            g.play();
        }
        if ((victory = g.getState()->isTerminal()) && !hasWon) {
            hasWon = true;
            text = "White win\n" + getVictoryMessage(victory);
        }

        win.drawBoard(*g.getState(), text);
        win.display();

        if (g.getTurn() == PlayerColor::blackPlayer && isBlackAI && !hasWon)
        {
            g.play();
        }
        if ((victory = g.getState()->isTerminal()) && !hasWon) {
            hasWon = true;
            text = "Black win\n" + getVictoryMessage(victory);
        }
        win.drawBoard(*g.getState(), text);
        win.display();
        usleep(2000);//better framerate system
    }
    usleep(5000000);
}

int menu_page(GuiManager& win)
{
    Game                g(false);

    while (1)
    {
        if (g.getState()->isTerminal())
            std::cout << "Win !!!" << std::endl;
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
                        return 0;
                    break ;
                case sf::Event::MouseButtonPressed:
                    auto mousePos = win.getMouseScreenRatio();
                    return (mousePos.y * 3 + 1);
            }
        }
        win.drawMenu();
        win.display();
        usleep(200);
    }
}

int main() {
    GuiManager          win;

    srand(time(NULL));
    while (1)
    {
        switch (menu_page(win))
        {
            case 0:
                return 0;
            case 1:
                game_page(win, true, true);
                break;
            case 2:
                game_page(win, true, false);
                break;
            case 3:
                game_page(win, false, false);
                break;
        }
    }
}