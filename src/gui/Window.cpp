#include "Window.hpp"
#include "raylib.h"
#include "cmath"

void runGameWindow(Board& board) {
    int width = 800;
    int height = 800;

    InitWindow(width, height, "Chess");
    loadPieceTextures();

    // For State management
    int c_file, c_rank;
    int c_square = -1;
    const char* message = nullptr;

    std::vector<Move> legalMoves;

    int pendingPromotionStart = -1;
    int pendingPromotionEnd = -1;

    PieceType promotionPieces[4] = {PieceType::Queen, PieceType::Rook, PieceType::Bishop, PieceType::Knight};

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw Board
        for(int rank = 0; rank < 8; rank++) {
            for(int file = 0; file < 8; file++) {
                Color color = ((rank + file) % 2 == 0) ? BROWN : BEIGE;

                DrawRectangle(file * 100, (7 - rank) * 100, 100, 100, color); // rank = 0 means the top in graphics
            }
        }


        // Highlighted Squares
        for(Move& move : legalMoves) {
            const PieceColor selectedColor = board.getPieceAt(move.start_square) -> getColor();

            if(selectedColor != board.getActiveColor()) break;

            int centerX = (move.end_square % 8) * 100 + 50; // Every square is 100 * 100
            int centerY = (7 - floor(move.end_square / 8)) * 100 + 50;

            Color color = Fade(GREEN, 0.5f);

            const Piece* endSquarePiece = board.getPieceAt(move.end_square);

            if(endSquarePiece == nullptr) {
                DrawCircle(centerX, centerY, 10, color);
            }else {
                Vector2 center = {centerX, centerY};
                DrawRing(center, 40, 45, 0, 360, 36, RED);
            }
        }

        // Place Pieces
        for(int square = 0; square < 64; square++) {
            const Piece* piece = board.getPieceAt(square);
            if(piece == nullptr) continue;

            Texture2D pieceTexture = getPieceTexture(piece -> getType(), piece -> getColor());
            Rectangle sourceRect = {0, 0, 128, 128};
            Rectangle destRec = {(square % 8) * 100, (7 - (square / 8)) * 100, 100, 100};
            DrawTexturePro(pieceTexture, sourceRect, destRec, {0, 0}, 0, WHITE);
        }

        // Promotion Pieces Drawing
        if(pendingPromotionStart >= 0) {
            int x = 2 * 100;
            int y = 3 * 100;

            DrawRectangle(x, y, 420, 200, WHITE);

            int i = 0;
            for(PieceType type : promotionPieces) {
                Texture2D pieceTexture = getPieceTexture(type, board.getActiveColor());
                Rectangle sourceRect = {0, 0, 128, 128};
                Rectangle destRec = {x + i++ * 100 + 20, y + 50, 100, 100};
                DrawTexturePro(pieceTexture, sourceRect, destRec, {0, 0}, 0, WHITE);
            }
        }
      
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && message == nullptr) {
            std::cout << "Click Detected" << std::endl;

            Vector2 mousePos = GetMousePosition();
            
            c_file = mousePos.x / 100;
            c_rank = 7 - floor(mousePos.y / 100);

            std::cout << c_file << " " << c_rank << std::endl;

            if(pendingPromotionStart < 0) {
                if(c_square < 0) {
                    // 1st Click
                    // Checks for right turn click
                    const Piece* selectedPiece = board.getPieceAt(c_rank * 8 + c_file); // c_square wasn't used as it had to be set under that condition
                    if(selectedPiece != nullptr && board.getActiveColor() == selectedPiece -> getColor()){
                        c_square = c_rank * 8 + c_file;
                        legalMoves = board.getLegalMovesForSquare(c_square);
                    }
                }else {
                    // 2nd Click
                    int endSquare = c_rank * 8 + c_file;

                    std::cout << c_square << " " << endSquare << std::endl;

                    std::cout << legalMoves.size() << std::endl;

                    for(Move& move : legalMoves) {
                        if(move.start_square == c_square && move.end_square == endSquare && move.moveType != MoveFlag::Promotion) {
                            board.playMove(move);

                            if(board.isCheckMate(board.getActiveColor())) {
                                message = (board.getActiveColor() == PieceColor::White) ? "Black Won" : "White won";
                            }

                            if(board.isStalemate(board.getActiveColor())) {
                                message = "Draw";
                            }

                            break;
                        }else if(move.start_square == c_square && move.end_square == endSquare && move.moveType == MoveFlag::Promotion){
                            pendingPromotionStart = c_square;
                            pendingPromotionEnd = endSquare;
                        }
                    }
                

                    // Need these variables for Promotion
                    if(pendingPromotionEnd < 0 && pendingPromotionStart < 0) {
                        legalMoves.clear();
                        c_square = -1;
                    }
                }
            }else {
                int x[4] = {220, 320, 420, 520};

                for(int i = 0; i < 4; i++) {
                    if((mousePos.x >= x[i] && mousePos.x < x[i] + 100) && (mousePos.y >= 400 && mousePos.y <= 500)) {
                        PieceType type = promotionPieces[i];

                        for(Move& move : legalMoves) {
                            if(move.start_square == pendingPromotionStart && move.end_square == pendingPromotionEnd && move.promotion == type) {
                                board.playMove(move);

                                if(board.isCheckMate(board.getActiveColor())) {
                                    message = (board.getActiveColor() == PieceColor::White) ? "Black Won" : "White won";
                                }

                                if(board.isStalemate(board.getActiveColor())) {
                                    message = "Draw";
                                }


                                break;
                            }
                        }

                        pendingPromotionStart = -1;
                        pendingPromotionEnd = -1;

                        legalMoves.clear();
                        c_square = -1;
                    }
                }
            }
        }

        if(message != nullptr) {
            int x = 2 * 100;
            int y = 3 * 100;

            DrawText(message, x, y, 100, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
}