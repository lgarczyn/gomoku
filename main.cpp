
#include <iostream>
#include "GuiManager.hpp"
#include "Game.hpp"
#include <unistd.h>

using namespace std;


void game_page(GuiManager& win, bool isBlackAI, bool isWhiteAI)
{
    Game                g(true);
    bool                hasWon = false;

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

                    if (g.getState()->isTerminal()) { hasWon = true; }

                    break ;
            }
        }

        if (g.getTurn() == PlayerColor::whitePlayer && isWhiteAI && !hasWon)
        {
            g.play();
        }
        if (g.getState()->isTerminal()) { hasWon = true; }

        if (g.getTurn() == PlayerColor::blackPlayer && isBlackAI && !hasWon)
        {
            g.play();
        }
        if (g.getState()->isTerminal()) { hasWon = true; }

        win.drawBoard(*g.getState(), hasWon);

        win.display();
        usleep(200);//better framerate system
    }
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

    while (1)
    {
        switch (menu_page(win))
        {
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