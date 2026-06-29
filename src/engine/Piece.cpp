#include "Piece.hpp"
#include "Board.hpp"
#include "Move.hpp"
#include "Types.hpp"
#include <vector>

std::vector<Move> Knight::getMoves(const Board &board, int square) const {
  std::vector<Move> moves; // Vector(array) of type Move

  int currentRank = getRank(square);
  int currentFile = getFile(square);

  const int rankOffsets[8] = {2, 2, -2, -2, 1, 1, -1, -1};
  const int fileOffsets[8] = {1, -1, 1, -1, 2, -2, 2, -2};

  for (int i = 0; i < 8; ++i) {
    int targetRank = currentRank + rankOffsets[i];
    int targetFile = currentFile + fileOffsets[i];

    if (targetRank < 0 || targetRank > 7 || targetFile < 0 || targetFile > 7)
      continue;

    int targetSquare = getSquare(targetRank, targetFile);

    const Piece *piece = board.getPieceAt(targetSquare);

    if (piece == nullptr) {
      moves.push_back({square, targetSquare, MoveFlag::Normal});
    } else if (piece->getColor() != this->getColor()) { // if target square's piece color != knight's color
      moves.push_back({square, targetSquare, MoveFlag::Normal});
    }
  }

  return moves;
}

std::vector<Move> Rook::getMoves(const Board &board, int square) const {
  std::vector<Move> moves;

  int currentRank = getRank(square);
  int currentFile = getFile(square);

  const int rankDir[4] = {1, -1, 0, 0};
  const int fileDir[4] = {0, 0, 1, -1};

  for (int d = 0; d < 4; d++) {
    for (int step = 1; step <= 7; ++step) {
      int targetRank = currentRank + rankDir[d] * step;
      int targetFile = currentFile + fileDir[d] * step;

      if (targetRank < 0 || targetRank > 7 || targetFile < 0 || targetFile > 7)
        break;

      int targetSquare = getSquare(targetRank, targetFile);
      const Piece *piece = board.getPieceAt(targetSquare);

      if (piece == nullptr) {
        moves.push_back({square, targetSquare, MoveFlag::Normal});
      } else if (piece->getColor() != this->getColor()) {
        moves.push_back({square, targetSquare, MoveFlag::Normal});
        break;
      } else
        break;
    }
  }

  return moves;
}

std::vector<Move> Bishop::getMoves(const Board &board, int square) const {
  std::vector<Move> moves;

  int currentRank = getRank(square);
  int currentFile = getFile(square);

  const int rankDir[4] = {1, 1, -1, -1};
  const int fileDir[4] = {1, -1, 1, -1};

  for (int d = 0; d < 4; d++) {
    for (int step = 1; step <= 7; ++step) {
      int targetRank = currentRank + rankDir[d] * step;
      int targetFile = currentFile + fileDir[d] * step;

      if (targetRank < 0 || targetRank > 7 || targetFile < 0 || targetFile > 7)
        break;

      int targetSquare = getSquare(targetRank, targetFile);
      const Piece *piece = board.getPieceAt(targetSquare);

      if (piece == nullptr) {
        moves.push_back({square, targetSquare, MoveFlag::Normal});
      } else if (piece->getColor() != this->getColor()) {
        moves.push_back({square, targetSquare, MoveFlag::Normal});
        break;
      } else
        break;
    }
  }

  return moves;
}

std::vector<Move> Queen::getMoves(const Board &board, int square) const {
  std::vector<Move> moves;

  int currentRank = getRank(square);
  int currentFile = getFile(square);

  const int rankDir[8] = {0, 1, 1, 1, 0, -1, -1, -1};
  const int fileDir[8] = {1, 1, 0, -1, -1, -1, 0, 1};

  for (int d = 0; d < 8; d++) {
    for (int step = 1; step <= 7; ++step) {
      int targetRank = currentRank + rankDir[d] * step;
      int targetFile = currentFile + fileDir[d] * step;

      if (targetRank < 0 || targetRank > 7 || targetFile < 0 || targetFile > 7)
        break;

      int targetSquare = getSquare(targetRank, targetFile);
      const Piece *piece = board.getPieceAt(targetSquare);

      if (piece == nullptr) {
        moves.push_back({square, targetSquare, MoveFlag::Normal});
      } else if (piece->getColor() != this->getColor()) {
        moves.push_back({square, targetSquare, MoveFlag::Normal});
        break;
      } else
        break;
    }
  }

  return moves;
}

std::vector<Move> Pawn::getMoves(const Board &board, int square) const {
  std::vector<Move> moves;

  int currentRank = getRank(square);
  int currentFile = getFile(square);
  PieceColor  pieceColor = this->getColor();

  int rankDir = (pieceColor == PieceColor ::White) ? 1 : -1;
  int targetRank = currentRank + rankDir;

  int enemyLastRank = (pieceColor == PieceColor ::White) ? 7 : 0;

  if (targetRank < 0 || targetRank > 7)
    return moves;

  // Single Step
  int targetSquare = getSquare(targetRank, currentFile);
  const Piece *singleStepPiece = board.getPieceAt(targetSquare);

  if (singleStepPiece == nullptr) {
    if(targetRank != enemyLastRank) moves.push_back({square, targetSquare, MoveFlag::Normal});
    else {
      moves.push_back({square, targetSquare, MoveFlag::Promotion, PieceType::Bishop});
      moves.push_back({square, targetSquare, MoveFlag::Promotion, PieceType::Knight});
      moves.push_back({square, targetSquare, MoveFlag::Promotion, PieceType::Queen});
      moves.push_back({square, targetSquare, MoveFlag::Promotion, PieceType::Rook});
    }

    // Double Step
    bool onStartingRank = (pieceColor == PieceColor ::White) ? (currentRank == 1) : (currentRank == 6);
    if (onStartingRank) {
      int doubleTargetRank = currentRank + (2 * rankDir);
      int doubleTargetSquare = getSquare(doubleTargetRank, currentFile);
      const Piece *doubleStepPiece = board.getPieceAt(doubleTargetSquare);

      if (doubleStepPiece == nullptr) {
        moves.push_back({square, doubleTargetSquare, MoveFlag::DoublePawnPush});
      }
    }
  }

  // Diagonal Captures and En Passant
  int fileOffsets[2] = {-1, 1};
  std::optional<int> enPassantSquare = board.getEnPassantSquare();

  for (int offset : fileOffsets) {
    int targetFile = currentFile + offset;

    if (targetFile < 0 || targetFile > 7) {
      continue;
    }

    int diagSquare = getSquare(targetRank, targetFile);
    const Piece *diagPiece = board.getPieceAt(diagSquare);

    // Diagonal Capture
    if (diagPiece != nullptr && diagPiece->getColor() != pieceColor) {
      if(targetRank != enemyLastRank) moves.push_back({square, diagSquare, MoveFlag::Normal});
      else {
        moves.push_back({square, diagSquare, MoveFlag::Promotion, PieceType::Bishop});
        moves.push_back({square, diagSquare, MoveFlag::Promotion, PieceType::Knight});
        moves.push_back({square, diagSquare, MoveFlag::Promotion, PieceType::Queen});
        moves.push_back({square, diagSquare, MoveFlag::Promotion, PieceType::Rook});
      }
    }
    // currentRank == ((pieceColor == PieceColor::White) ? 4 : 3) this checks if you're on the correct rank for your color
    else if (enPassantSquare.has_value() && enPassantSquare.value() == diagSquare && currentRank == ((pieceColor == PieceColor::White) ? 4 : 3)) { // En Passant Capture
      moves.push_back({square, diagSquare, MoveFlag::EnPassant});
    }
  }

  return moves;
}

std::vector<Move> King::getMoves(const Board& board, int square) const {
  std::vector<Move> moves;

  int currentRank = getRank(square);
  int currentFile = getFile(square);

  const int rankDir[8] = {0, 1, 1, 1, 0, -1, -1, -1};
  const int fileDir[8] = {1, 1, 0, -1, -1, -1, 0, 1};

  // Normal moves
  for (int d = 0; d < 8; d++) {
    int targetRank = currentRank + rankDir[d];
    int targetFile = currentFile + fileDir[d];

    if (targetFile < 0 || targetFile > 7 || targetRank < 0 || targetRank > 7)
      continue;

    int targetSquare = getSquare(targetRank, targetFile);
    const Piece *piece = board.getPieceAt(targetSquare);

    if (piece == nullptr) {
      moves.push_back({square, targetSquare, MoveFlag::Normal});
    } else if (piece->getColor() != this->getColor()) {
      moves.push_back({square, targetSquare, MoveFlag::Normal});
    }
  }

  // Castling
  PieceColor  color = this -> getColor();

  bool inCheck = board.isInCheck(color);
  PieceColor  byColor = (color == PieceColor ::White) ? PieceColor ::Black : PieceColor ::White;

  // King side
  if((color == PieceColor ::White && board.canWhiteCastleKingside() && !inCheck) || (color == PieceColor ::Black && board.canBlackCastleKingside() && !inCheck)) {
    int f_square = getSquare(currentRank, 5);
    int g_square = getSquare(currentRank, 6);


    if(board.getPieceAt(f_square) == nullptr && board.getPieceAt(g_square) == nullptr && !board.isSquareAttacked(f_square, byColor) && !board.isSquareAttacked(g_square, byColor)) { // f and g file
      moves.push_back({square, g_square, MoveFlag::Castling});
    }
  }

  // Queen side
  if((color == PieceColor ::White && board.canWhiteCastleQueenside() && !inCheck) || (color == PieceColor ::Black && board.canBlackCastleQueenside() && !inCheck)) {
    int c_square = getSquare(currentRank, 2);
    int d_square = getSquare(currentRank, 3);

    int b_square = getSquare(currentRank, 1); // For checking if it is empty

    if(board.getPieceAt(c_square) == nullptr && board.getPieceAt(b_square) == nullptr && board.getPieceAt(d_square) == nullptr && !board.isSquareAttacked(c_square, byColor) && !board.isSquareAttacked(d_square, byColor)) { // f and g file
      moves.push_back({square, c_square, MoveFlag::Castling});
    }
  }

  return moves;
}