#pragma once

#include "Types.hpp"
#include "Move.hpp"
#include <vector>

class Board;
// struct Move;

class Piece {
    private:
        PieceColor  m_color;
        PieceType m_type;
    
    public:
        Piece(PieceColor  color, PieceType type) : m_color(color), m_type(type) {}
        /*
            Piece(PieceColor  color, PieceType type) {
                m_color = color;
                m_type = type;
            }
        */
        virtual ~Piece() = default;

        PieceColor  getColor() const { return m_color; } // read only
        PieceType getType() const { return m_type; } // read only

        virtual std::vector<Move> getMoves(const Board& board, int square) const = 0; // Gets all the legal moves
};

class Pawn : public Piece { // inheritance
    public:
        Pawn(PieceColor  color) : Piece(color, PieceType::Pawn) {}
        std::vector<Move> getMoves(const Board& board, int square) const override;
};

class Knight : public Piece {
    public:
        Knight(PieceColor  color) : Piece(color, PieceType::Knight) {}
        std::vector<Move> getMoves(const Board& board, int square) const override;
};

class Bishop : public Piece {
    public:
        Bishop(PieceColor  color) : Piece(color, PieceType::Bishop) {}
        std::vector<Move> getMoves(const Board& board, int square) const override;
};

class Rook : public Piece {
    public:
        Rook(PieceColor  color) : Piece(color, PieceType::Rook) {}
        std::vector<Move> getMoves(const Board& board, int square) const override;
};

class Queen : public Piece {
    public:
        Queen(PieceColor  color) : Piece(color, PieceType::Queen) {}
        std::vector<Move> getMoves(const Board& board, int square) const override;
};

class King : public Piece {
    public:
        King(PieceColor  color) : Piece(color, PieceType::King) {}
        std::vector<Move> getMoves(const Board& board, int square) const override;
};