#include "Board.hpp"

// Helpers
struct PieceSetup {
    Square sq;
    PieceType type;
    PieceColor  color;
};

PieceSetup setup[32] = {
    // White pieces
    {Square::a1, PieceType::Rook,   PieceColor ::White},
    {Square::b1, PieceType::Knight, PieceColor ::White},
    {Square::c1, PieceType::Bishop, PieceColor ::White},
    {Square::d1, PieceType::Queen,  PieceColor ::White},
    {Square::e1, PieceType::King,   PieceColor ::White},
    {Square::f1, PieceType::Bishop, PieceColor ::White},
    {Square::g1, PieceType::Knight, PieceColor ::White},
    {Square::h1, PieceType::Rook,   PieceColor ::White},

    {Square::a2, PieceType::Pawn, PieceColor ::White},
    {Square::b2, PieceType::Pawn, PieceColor ::White},
    {Square::c2, PieceType::Pawn, PieceColor ::White},
    {Square::d2, PieceType::Pawn, PieceColor ::White},
    {Square::e2, PieceType::Pawn, PieceColor ::White},
    {Square::f2, PieceType::Pawn, PieceColor ::White},
    {Square::g2, PieceType::Pawn, PieceColor ::White},
    {Square::h2, PieceType::Pawn, PieceColor ::White},

    // Black pieces
    {Square::a8, PieceType::Rook,   PieceColor ::Black},
    {Square::b8, PieceType::Knight, PieceColor ::Black},
    {Square::c8, PieceType::Bishop, PieceColor ::Black},
    {Square::d8, PieceType::Queen,  PieceColor ::Black},
    {Square::e8, PieceType::King,   PieceColor ::Black},
    {Square::f8, PieceType::Bishop, PieceColor ::Black},
    {Square::g8, PieceType::Knight, PieceColor ::Black},
    {Square::h8, PieceType::Rook,   PieceColor ::Black},

    {Square::a7, PieceType::Pawn, PieceColor ::Black},
    {Square::b7, PieceType::Pawn, PieceColor ::Black},
    {Square::c7, PieceType::Pawn, PieceColor ::Black},
    {Square::d7, PieceType::Pawn, PieceColor ::Black},
    {Square::e7, PieceType::Pawn, PieceColor ::Black},
    {Square::f7, PieceType::Pawn, PieceColor ::Black},
    {Square::g7, PieceType::Pawn, PieceColor ::Black},
    {Square::h7, PieceType::Pawn, PieceColor ::Black},
};

std::unique_ptr<Piece> createPiece(PieceType type, PieceColor  color) {
    switch (type) {
        case PieceType::Pawn:   return std::make_unique<Pawn>(color);
        case PieceType::Knight: return std::make_unique<Knight>(color);
        case PieceType::Bishop: return std::make_unique<Bishop>(color);
        case PieceType::Rook:   return std::make_unique<Rook>(color);
        case PieceType::Queen:  return std::make_unique<Queen>(color);
        case PieceType::King:   return std::make_unique<King>(color);
        default: return nullptr;
    }
}


Board::Board() : m_activeColor(PieceColor ::White),
                m_whiteCastleQueenside(true),
                m_whiteCastleKingside(true),
                m_blackCastleKingside(true),
                m_blackCastleQueenside(true),
                en_passant_square(std::nullopt)
{

}

void Board::placePiece(PieceType type, PieceColor  color, Square square) {
    m_square[toInt(square)] = createPiece(type, color);
}

void Board::printBoard() const {
    for(int rank = 7; rank >= 0; rank--) {
        std::cout << rank + 1 << " ";
        for(int file = 0; file <= 7; file++) {
            int square = getSquare(rank, file);
            const Piece* piece = getPieceAt(square);

            if(!piece) std::cout << ". ";
            else {
                char c = "PBNRQK"[static_cast<int>(piece -> getType())]; // gets the type of piece in char
                if(piece -> getColor() == PieceColor ::Black) c = std::tolower(c);
                
                std::cout << c << " ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << " a b c d e f g h" << std::endl;
}

// Game controllers
void Board::setupStartingPosition() {
    // Reseting
    for(int i = 0; i < 64; i++) {
        m_square[i] = nullptr;
    }

    // Placing Pieces
    for(PieceSetup p : setup) {
        placePiece(p.type, p.color, p.sq);
    }

    // Game state init
    m_activeColor = PieceColor ::White;
    m_whiteCastleKingside = m_whiteCastleQueenside = true;
    m_blackCastleKingside = m_blackCastleQueenside = true;
    en_passant_square = std::nullopt;
}

const Piece* Board::getPieceAt(int square) const {
    return m_square[square].get();
}


int Board::findKing(PieceColor  color) const {
    for(int sq = 0; sq < 64; sq++) {
        const Piece* piece = getPieceAt(sq);

        if(piece == nullptr) continue;

        if(piece -> getType() == PieceType::King && piece -> getColor() == color) {
            return sq;
        }
    }

    return -1;
}


bool Board::isSquareAttacked(int targetSquare, PieceColor  byColor) const {
    int targetRank = getRank(targetSquare);
    int targetFile = getFile(targetSquare);

    // Queen and Bishop attack
    const int queenRankDir[4] = {1, 1, -1, -1};
    const int queenFileDir[4] = {1, -1, 1, -1};
    for(int d = 0; d < 4; d++) {
        for(int step = 1; step <= 7; step++) {
            int pieceRank = targetRank + step * queenRankDir[d];
            int pieceFile = targetFile + step * queenFileDir[d];
            
            if(pieceRank < 0 || pieceRank > 7 || pieceFile < 0 || pieceFile > 7) break;
            
            int pieceSquare = getSquare(pieceRank, pieceFile);
            const Piece* piece = getPieceAt(pieceSquare);
            
            if(piece == nullptr) continue;
            else if(piece -> getColor() == byColor && (piece -> getType() == PieceType::Bishop || piece -> getType() == PieceType::Queen)) return true;
            else break;
        }
    }

    // Queen and Rook attack
    const int rookRankDir[4] = {1, -1, 0, 0};
    const int rookFileDir[4] = {0, 0, 1, -1};
    for(int d = 0; d < 4; d++) {
        for(int step = 1; step <= 7; step++) {
            int pieceRank = targetRank + step * rookRankDir[d];
            int pieceFile = targetFile + step * rookFileDir[d];
            
            if(pieceRank < 0 || pieceRank > 7 || pieceFile < 0 || pieceFile > 7) break;
            
            int pieceSquare = getSquare(pieceRank, pieceFile);
            const Piece* piece = getPieceAt(pieceSquare);
            
            if(piece == nullptr) continue;
            else if(piece -> getColor() == byColor && (piece -> getType() == PieceType::Rook || piece -> getType() == PieceType::Queen)) return true;
            else break;
        }
    }

    // Knight attack
    const int knightRankDir[8] = {2, 2, -2, -2, 1, 1, -1, -1};
    const int knightFileDir[8] = {1, -1, 1, -1, 2, -2, 2, -2};
    for(int d = 0; d < 8; d++) {
        int pieceRank = targetRank + knightRankDir[d];
        int pieceFile = targetFile + knightFileDir[d];
        
        if(pieceRank < 0 || pieceRank > 7 || pieceFile < 0 || pieceFile > 7) continue;
        
        int pieceSquare = getSquare(pieceRank, pieceFile);
        const Piece* piece = getPieceAt(pieceSquare);
        
        if(piece == nullptr) continue;
        else if(piece -> getColor() == byColor && piece -> getType() == PieceType::Knight) return true;
        else continue;
    }

    // Pawn attack
    const int pawnRankDir = (byColor == PieceColor ::White) ? -1 : 1; // Think from black King
    const int pawnFileDir[2] = {-1, 1};

    for(int fileDir : pawnFileDir) {
        int pieceRank = targetRank + pawnRankDir;
        int pieceFile = targetFile + fileDir;

        if(pieceRank < 0 || pieceRank > 7 || pieceFile < 0 || pieceFile > 7) continue;

        int pieceSquare = getSquare(pieceRank, pieceFile);
        const Piece* piece = getPieceAt(pieceSquare);
        
        if(piece == nullptr) continue;
        else if(piece -> getColor() == byColor && piece -> getType() == PieceType::Pawn) return true;
        else continue;
    }

    // King attack
    const int kingRankDir[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    const int kingFileDir[8] = {1, 1, 0, -1, -1, -1, 0, 1};

    for(int d = 0; d < 8; d++) {
        int pieceRank = targetRank + kingRankDir[d];
        int pieceFile = targetFile + kingFileDir[d];
        
        if(pieceRank < 0 || pieceRank > 7 || pieceFile < 0 || pieceFile > 7) continue;
        
        int pieceSquare = getSquare(pieceRank, pieceFile);
        const Piece* piece = getPieceAt(pieceSquare);
        
        if(piece == nullptr) continue;
        else if(piece -> getColor() == byColor && piece -> getType() == PieceType::King) return true;
        else continue;
    }

    return false;
}


bool Board::isInCheck(PieceColor  color) const {
    const int kingSquare = findKing(color);

    PieceColor  attackerColor = (color == PieceColor ::White) ? PieceColor ::Black : PieceColor ::White;
    bool kingCheck = isSquareAttacked(kingSquare, attackerColor);

    return kingCheck;
}


UndoInfo Board::makeMove(const Move& move) {
    // std::move() just transfers ownership of the unique_ptr. So now piece var owns the start square piece pointer. m_square[start] becomes nullptr
    std::unique_ptr piece = std::move(m_square[move.start_square]); // Moving Piece

    UndoInfo info = {false, PieceType::None, PieceColor ::White, false, false, false, false, en_passant_square};

    en_passant_square = std::nullopt; // Resetting the square

    if(!piece) return info;


    // For Undo
    PieceType capturedType;
    if(m_square[move.end_square] != nullptr) {
        capturedType = m_square[move.end_square] -> getType();
        info.capturedPieceType = capturedType;
    }else {
        capturedType = PieceType::None;
        info.capturedPieceType = capturedType;
    }

    if(capturedType != PieceType::None) {
        PieceColor  capturedColor = m_square[move.end_square] -> getColor();
        info.capturedPieceColor = capturedColor;
    }

    info.success = true;

    info.prevWhiteKingSide = m_whiteCastleKingside;
    info.prevWhiteQueenSide = m_whiteCastleQueenside;
    info.prevBlackKingSide = m_blackCastleKingside;
    info.prevBlackQueenSide = m_blackCastleQueenside;

    switch(move.moveType) {
        case MoveFlag::Normal:   
            // Castling rights
            if(piece -> getType() == PieceType::King && piece -> getColor() == PieceColor ::White) {
                m_whiteCastleKingside = m_whiteCastleQueenside = false;
            }else if(piece -> getType() == PieceType::King && piece -> getColor() == PieceColor ::Black) {
                m_blackCastleKingside = m_blackCastleQueenside = false;
            }

            //White rooks castle
            if(piece -> getType() == PieceType::Rook && piece -> getColor() == PieceColor ::White && move.start_square == static_cast<int>(Square::a1)){
                m_whiteCastleQueenside = false;
            }else if(piece -> getType() == PieceType::Rook && piece -> getColor() == PieceColor ::White && move.start_square == static_cast<int>(Square::h1)){
                m_whiteCastleKingside = false;
            }

            // Black rooks castle
            if(piece -> getType() == PieceType::Rook && piece -> getColor() == PieceColor ::Black && move.start_square == static_cast<int>(Square::a8)){
                m_blackCastleQueenside = false;
            }else if(piece -> getType() == PieceType::Rook && piece -> getColor() == PieceColor ::Black && move.start_square == static_cast<int>(Square::h8)){
                m_blackCastleKingside = false;
            }

            m_square[move.end_square] = std::move(piece); // Moving the piece

            break;
        case MoveFlag::DoublePawnPush:
            en_passant_square = move.end_square + ((piece->getColor() == PieceColor ::White) ? -8 : +8);
            m_square[move.end_square] = std::move(piece);

            break;
        case MoveFlag::Castling: {
            int kingEndSq = move.end_square;
            int rookStartSq;
            int rookEndSq;

            if(kingEndSq == 6) {
                rookStartSq = 7;
                rookEndSq = 5;
            }else if(kingEndSq == 2) {
                rookStartSq = 0;
                rookEndSq = 3;
            }else if(kingEndSq == 62) {
                rookStartSq = 63;
                rookEndSq = 61;
            }else if(kingEndSq == 58) {
                rookStartSq = 56;
                rookEndSq = 59;
            }

            std::unique_ptr rook = std::move(m_square[rookStartSq]);
            
            if(piece -> getColor() == PieceColor ::White) {
                m_whiteCastleKingside = m_whiteCastleQueenside = false;
            }else {
                m_blackCastleKingside = m_blackCastleQueenside = false;
            }
            
            m_square[kingEndSq] = std::move(piece); // King move
            m_square[rookEndSq] = std::move(rook); // Rook move
            break;
        }
        case MoveFlag::EnPassant: {
            int enemySquare = move.end_square + ((piece -> getColor() == PieceColor ::White) ? -8 : 8);

            m_square[move.end_square] = std::move(piece);
            m_square[enemySquare] = nullptr;

            info.capturedPieceType = PieceType::Pawn;
            info.capturedPieceColor = (m_square[move.end_square] -> getColor() == PieceColor ::White) ? PieceColor ::Black : PieceColor ::White;

            break;
        }
        case MoveFlag::Promotion: {
            std::unique_ptr promoted = createPiece(move.promotion, piece -> getColor());
            m_square[move.end_square] = std::move(promoted);

            break;
        }
            
    }
    m_activeColor = (m_activeColor == PieceColor ::White) ? PieceColor ::Black : PieceColor ::White;

    return info;
}


void Board::undoMove(const Move& move, const UndoInfo& info) {

    
    en_passant_square = info.prevEnPassantSquare;
    
    m_whiteCastleKingside = info.prevWhiteKingSide;
    m_whiteCastleQueenside = info.prevWhiteQueenSide;
    m_blackCastleKingside = info.prevBlackKingSide;
    m_blackCastleQueenside = info.prevBlackQueenSide;

    switch(move.moveType) {
        case MoveFlag::Normal: {
            std::unique_ptr<Piece> piece = std::move(m_square[move.end_square]);

            m_square[move.start_square] = std::move(piece); // Moving back

            //Capture
            if(info.capturedPieceType != PieceType::None) {
                std::unique_ptr capturedPiece = createPiece(info.capturedPieceType, info.capturedPieceColor);

                m_square[move.end_square] = std::move(capturedPiece);
            }
        
            break;
        }
        case MoveFlag::DoublePawnPush: {
            std::unique_ptr<Piece> piece = std::move(m_square[move.end_square]);

            m_square[move.start_square] = std::move(piece);

            break;
        }
        case MoveFlag::Castling: {
            std::unique_ptr<Piece> king = std::move(m_square[move.end_square]);
            
            m_square[move.start_square] = std::move(king); // Resets king

            // For rook
            int kingEndSq = move.end_square;
            int rookStartSq;
            int rookEndSq;

            if(kingEndSq == 6) {
                rookStartSq = 7;
                rookEndSq = 5;
            }else if(kingEndSq == 2) {
                rookStartSq = 0;
                rookEndSq = 3;
            }else if(kingEndSq == 62) {
                rookStartSq = 63;
                rookEndSq = 61;
            }else if(kingEndSq == 58) {
                rookStartSq = 56;
                rookEndSq = 59;
            }

            std::unique_ptr<Piece> rook = std::move(m_square[rookEndSq]);

            m_square[rookStartSq] = std::move(rook);

            break;
        }
        case MoveFlag::EnPassant: {
            std::unique_ptr<Piece> attacker = std::move(m_square[move.end_square]);

            m_square[move.start_square] = std::move(attacker); // attacker reset

            // For Captured
            int enemySquare = move.end_square + ((m_square[move.start_square] -> getColor() == PieceColor ::White) ? -8 : 8);
            std::unique_ptr<Piece> captured = createPiece(info.capturedPieceType, info.capturedPieceColor);

            m_square[enemySquare] = std::move(captured);

            break;
        }
        case MoveFlag::Promotion: {
            std::unique_ptr<Piece> promoted = std::move(m_square[move.end_square]);

            PieceColor  pawnColor = promoted -> getColor();

            //Capture
            if(info.capturedPieceType != PieceType::None) {
                std::unique_ptr capturedPiece = createPiece(info.capturedPieceType, info.capturedPieceColor);

                m_square[move.end_square] = std::move(capturedPiece);
            }

            std::unique_ptr<Piece> pawn = createPiece(PieceType::Pawn, pawnColor);
            m_square[move.start_square] = std::move(pawn);

            break;
        }
    }

    m_activeColor = (m_activeColor == PieceColor ::White) ? PieceColor ::Black : PieceColor ::White;
}

std::vector<Move> Board::getLegalMovesForSquare(int square) {
    std::vector<Move> legalMoves;

    const Piece* piece = getPieceAt(square);
    
    if(piece == nullptr) return legalMoves;
    
    const PieceColor  pieceColor = piece -> getColor();
    std::vector<Move> pseudoMoves = piece -> getMoves(*this, square);

    for(Move& move : pseudoMoves) {
        UndoInfo info = makeMove(move);

        if(isInCheck(pieceColor) && info.success) {
            undoMove(move, info);
            continue;
        }
        
        legalMoves.push_back(move);
        undoMove(move, info);
    }

    return legalMoves;
}

std::vector<Move> Board::getLegalMoves(PieceColor  color) {
    std::vector<Move> allLegalMoves;

    // Check all the squares
    for(int square = 0; square < 64; square++) {
        const Piece* piece = getPieceAt(square);
        if(piece == nullptr) continue;
        
        const PieceColor  pieceColor = piece -> getColor();
        if(pieceColor != color) continue;

        std::vector<Move> legalMoves = getLegalMovesForSquare(square); // Get legal moves for a piece of that color

        allLegalMoves.insert(allLegalMoves.end(), legalMoves.begin(), legalMoves.end());
    }

    return allLegalMoves;
}

bool Board::isCheckMate(PieceColor  color) {
    std::vector<Move> allLegalMoves = getLegalMoves(color);
    
    if(isInCheck(color) && allLegalMoves.empty()) return true;

    return false;
}

bool Board::isStalemate(PieceColor  color) {
    std::vector<Move> allLegalMoves = getLegalMoves(color);
    
    if(!isInCheck(color) && allLegalMoves.empty()) return true;

    return false;
}

// For GUI
bool Board::playMove(const Move& move) {
    const Piece* piece = getPieceAt(move.start_square);

    if(piece && piece -> getColor() == m_activeColor) {
        UndoInfo info = makeMove(move);
    
        if(info.success) {
            MoveHistory history = {move, info};
            moveHistory.push_back(history);
    
            return true;
        }
    }

    return false;
}

void Board::undoLastMove() {
    if(moveHistory.empty()) return;

    MoveHistory lastHistory = moveHistory.back();
    undoMove(lastHistory.move, lastHistory.undoInfo);
    
    moveHistory.pop_back();
}