
#include <iostream>
#include "GuiManager.hpp"
#include "Game.hpp"
#include <unistd.h>

using namespace std;

int main() {
    GuiManager          win;
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
                        exit(0);
                    break ;
                case sf::Event::MouseButtonPressed:
                    BoardPos pos;
                    if (win.getMouseBoardPos(pos))
                    {
                        if (g.play(pos))
                        {
                            g.play();
                        }
                    }
                    break ;
            }
        }
        win.drawBoard(*g.getState());

        win.display();
        usleep(200);
    }
    return 0;
}