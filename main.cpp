
#include <iostream>
#include "GuiManager.hpp"
#include "Board.hpp"


#include <unistd.h>
using namespace std;

int main() {
    Board   b;
    GuiManager          win(1200, 900, "Gomoku");

    while (1)
    {
        if (b.isTerminal())
            std::cout << "Win !!!" << std::endl;
        win.clear();
        sf::Event   event;
        while (win.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::Closed:
                    exit(0);
                    break ;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape)
                        exit(0);
                    break ;
                case sf::Event::MouseButtonPressed:
                {
                    int     cell_width = win.getCellWidth();
                    int     x = event.mouseButton.x / cell_width;
                    int     y = event.mouseButton.y / cell_width;

                    if (event.mouseButton.button == sf::Mouse::Left)
                        (*b.getData())[y][x] = BoardSquare::white;
                    if (event.mouseButton.button == sf::Mouse::Right)
                        (*b.getData())[y][x] = BoardSquare::black;
                }
                    break ;
            }
        }
        win.drawBoard(b);

        win.display();
        usleep(200);
    }
    return 0;
}