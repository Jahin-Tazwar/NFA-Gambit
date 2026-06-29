#pragma once

#include "Assets.hpp"
#include "Board.hpp"

void runGameWindow(Board& board); // Not const as it will have to call board.playMove();