# NFA's Gambit

A C++ chess engine and GUI built from scratch as a long-term learning project — every line of engine and GUI code is hand-written, with no AI-generated or copy-pasted implementation logic.

## Current Status

Fully playable two-player (human vs. human) chess, with a clickable GUI built on [raylib](https://www.raylib.com/).

### Working features

- **Move generation** for all piece types, including castling and en passant
- **Legal move filtering** (pins, check, castling-through-check all accounted for)
- **Check / checkmate / stalemate detection**
- **Move history** with undo support (engine-side)
- **Click-to-move GUI**: select a piece, see its legal destinations highlighted live, click to move
- **Turn enforcement** — you can only move your own pieces on your own turn
- **Pawn promotion** with an in-game piece picker (Queen / Rook / Bishop / Knight)
- **Capture highlighting** distinct from regular move highlighting

### Not yet implemented

- AI opponent (minimax / alpha-beta search) — in progress
- Redo (only undo currently exists)
- On-screen turn indicator

See [`PROJECT.md`](PROJECT.md) for the full learning roadmap and phase breakdown.

## Project Structure

```
src/
  engine/         Pure chess logic — no GUI dependencies
    Types.hpp     Core enums (PieceColor, PieceType, Square) and square/rank/file helpers
    Piece.hpp/cpp Per-piece move generation (Pawn, Knight, Bishop, Rook, Queen, King)
    Move.hpp/cpp  Move, UndoInfo, and MoveHistory data structures
    Board.hpp/cpp Board state, move execution/undo, legality filtering, game-end detection
  gui/            Visual layer — depends on the engine, never the reverse
    Window.hpp/cpp  Main game loop: rendering, click handling, promotion UI
    Assets.hpp/cpp  Piece texture loading
  assets/pieces/  Piece sprite images (PNG)
  main.cpp        Entry point
CMakeLists.txt
```

## Building

### Requirements

- CMake 3.16+
- A C++17 compiler (developed with the [w64devkit](https://github.com/skeeto/w64devkit) MinGW toolchain on Windows)
- [raylib](https://www.raylib.com/) — this project currently expects raylib at `C:/raylib/w64devkit` (see `CMakeLists.txt`); adjust the include/lib paths there if your raylib install lives elsewhere

### Build & run

```sh
cmake -S . -B build
cmake --build build
./build/chess-game.exe
```

Run from the project root (not from inside `build/`) — piece sprites are loaded via paths relative to the project root (`./src/assets/pieces/...`).

## Architecture

The engine (`src/engine`) and GUI (`src/gui`) are kept strictly separated: the engine has no knowledge of raylib or rendering, and the GUI never implements chess rules — it only calls into the engine's public API (`getLegalMovesForSquare`, `playMove`, `isCheckMate`, etc.) and renders the result.
