#include "Board.hpp"
#include "Window.hpp"

#include <string>

int main() {
    Board board;
    board.setupStartingPosition();

    runGameWindow(board);
    return 0;
}