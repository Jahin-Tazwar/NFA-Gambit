#pragma once

#include <iostream>
#include <memory>
#include <optional>

#include "Piece.hpp"


class Board {
    private:
        std::unique_ptr<Piece> m_square[64]; // Each square exclusively owns 1 piece object (automatic memory handling when piece moves)
        std::vector<MoveHistory> moveHistory;

        PieceColor  m_activeColor;

        bool m_whiteCastleKingside;
        bool m_whiteCastleQueenside;

        bool m_blackCastleKingside;
        bool m_blackCastleQueenside;

        std::optional<int> en_passant_square;
    public:
        Board();
        
        const Piece* getPieceAt(int square) const;
        PieceColor  getActiveColor() const { return m_activeColor; }

        bool canWhiteCastleKingside() const { return m_whiteCastleKingside; }
        bool canWhiteCastleQueenside() const { return m_whiteCastleQueenside; }

        bool canBlackCastleKingside() const { return m_blackCastleKingside; }
        bool canBlackCastleQueenside() const { return m_blackCastleQueenside; }

        bool isSquareAttacked(int targetSquare, PieceColor  byColor) const;
        bool isInCheck(PieceColor  color) const;

        bool isCheckMate(PieceColor  color);
        bool isStalemate(PieceColor  color);

        std::vector<Move> getLegalMovesForSquare(int square); // All legal moves for a piece
        std::vector<Move> getLegalMoves(PieceColor  color); // All legal moves for a color

        std::optional<int> getEnPassantSquare() const { return en_passant_square; }

        // Game controller Methods
        void setupStartingPosition();
        void placePiece(PieceType type, PieceColor  color, Square square);

        UndoInfo makeMove(const Move& move);
        void undoMove(const Move& move, const UndoInfo& info);

        // For GUI
        bool playMove(const Move& move);
        void undoLastMove();

        //Helpers
        int findKing(PieceColor  color) const;

        // Console board
        void printBoard() const;
};