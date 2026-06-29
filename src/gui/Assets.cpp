#include "Assets.hpp"

Texture2D textures[2][6];

void loadPieceTextures() {
    // Black pieces
    int color = static_cast<int>(PieceColor::Black);
    textures[color][static_cast<int> (PieceType::Bishop)] = LoadTexture("./src/assets/pieces/black-bishop.png");
    textures[color][static_cast<int> (PieceType::King)] = LoadTexture("./src/assets/pieces/black-king.png");
    textures[color][static_cast<int> (PieceType::Knight)] = LoadTexture("./src/assets/pieces/black-knight.png");
    textures[color][static_cast<int> (PieceType::Pawn)] = LoadTexture("./src/assets/pieces/black-pawn.png");
    textures[color][static_cast<int> (PieceType::Queen)] = LoadTexture("./src/assets/pieces/black-queen.png");
    textures[color][static_cast<int> (PieceType::Rook)] = LoadTexture("./src/assets/pieces/black-rook.png");

    //White Pieces
    color = static_cast<int> (PieceColor::White);
    textures[color][static_cast<int> (PieceType::Bishop)] = LoadTexture("./src/assets/pieces/white-bishop.png");
    textures[color][static_cast<int> (PieceType::King)]  = LoadTexture("./src/assets/pieces/white-king.png");
    textures[color][static_cast<int> (PieceType::Knight)] = LoadTexture("./src/assets/pieces/white-knight.png");
    textures[color][static_cast<int> (PieceType::Pawn)]  = LoadTexture("./src/assets/pieces/white-pawn.png");
    textures[color][static_cast<int> (PieceType::Queen)] = LoadTexture("./src/assets/pieces/white-queen.png");
    textures[color][static_cast<int> (PieceType::Rook)]  = LoadTexture("./src/assets/pieces/white-rook.png");
}

Texture2D getPieceTexture(PieceType type, PieceColor color) {
    int pieceType = static_cast<int> (type);
    int pieceColor = static_cast<int> (color);

    return textures[pieceColor][pieceType];
}