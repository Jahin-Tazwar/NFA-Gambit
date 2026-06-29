#pragma once

#include "Types.hpp"
#include <optional>

enum class MoveFlag {
    Normal,
    DoublePawnPush,
    EnPassant,
    Castling,
    Promotion
};

struct Move{
    int start_square;
    int end_square;

    MoveFlag moveType;
    PieceType promotion = PieceType::None;
};


struct UndoInfo {
    bool success;

    PieceType capturedPieceType;
    PieceColor  capturedPieceColor;

    bool prevWhiteKingSide;
    bool prevWhiteQueenSide;
    bool prevBlackKingSide;
    bool prevBlackQueenSide;

    std::optional<int> prevEnPassantSquare;
};

struct MoveHistory {
    Move move;
    UndoInfo undoInfo;
};