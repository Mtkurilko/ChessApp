# ChessApp — One-Day Fun Project

This is a small, one-day C++ + SFML chess experiment I built for fun and learning. Nothing fancy — just a compact implementation of board state, piece movement rules, and a basic SFML renderer.

Quick notes:
- Purpose: experiment with chess logic and SFML rendering in a single afternoon.
- Not intended for production; rough edges expected (no en passant, promotion is basic queen autoselection).
- No contribution RFC — it’s a personal toy project.

## Quick Start (macOS)
Prereqs: clang (Command Line Tools) and SFML 3.x

Install SFML:
```bash
brew install sfml
```

Build and run:
```bash
bash ./compile.sh
./chess_app
```

## What’s Included
- Minimal board & piece logic in `Board*.cpp/h` and `Piece*.cpp/h`
- Game orchestration in `Game.cpp`
- Simple SFML rendering and sprites in `sprites/`

## Known Limitations
- Piece slicing due to `Piece` being returned by value from `Board`
- Simplified pawn logic, no en passant, promotion flow needs improvement
- No tests or CI (this is a weekend experiment)

## If You Want To Tinker
- Replace `sprites/` with your own piece images
- Add promotion UI or implement castling rules
- Swap board storage to polymorphic pointers to avoid slicing

Enjoy — quick and messy, as intended. ♟
