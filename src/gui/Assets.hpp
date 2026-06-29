#pragma once

#include "Types.hpp"

#include <raylib.h>

void loadPieceTextures();
Texture2D getPieceTexture(PieceType type, PieceColor color);