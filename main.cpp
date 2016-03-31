
#include <iostream>
#include "GuiManager.hpp"
#include "Board.hpp"


#include <unistd.h>
using namespace std;

int main() {
    Board   b;
    GuiManager          win(1200, 900, "gomoku");

    (*b.getData())[1][1] = BoardSquare::white;
    while (1)
    {
        win.clear();
        sf::Event   event;
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                exit(0);
        }
        win.draw_board(b);

        win.display();
        sleep(1);
    }
    return 0;
}