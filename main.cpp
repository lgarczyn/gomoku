
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
        win.clear();
        sf::Event   event;
        while (win.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::Closed:
                    exit(0);
                    break ;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        int     cell_width = win.getCellWidth();
                        int     x = event.mouseButton.x / cell_width;
                        int     y = event.mouseButton.y / cell_width;

                        (*b.getData())[y][x] = BoardSquare::white;
                    }
                    break ;
            }
        }
        win.drawBoard(b);

        win.display();
        sleep(1);
    }
    return 0;
}