#pragma once

#include "Game.h"

#include "MoveHandler.h"
#include "Vector2D.h"
#include <vector>
#include <unordered_map>


class Board {

public:
    Board() {
        SDL_Log("Creating board...\n");
    };
    ~Board();

    //  Static instance of the board
    static uint32_t board[Chess::Tiles::TILE_HEIGHT][Chess::Tiles::TILE_WIDTH];
    static SDL_Texture* spriteSheet;

    //  Get instance
    static Board*& getInstance() {
        static Board* _self = new Board();
        return _self;
    }

    //  Methods

    void InitBoard(Player*& player, Player*& enemy);

    uint32_t& CheckForPiece(Vector2D& position);
    
    static void DrawPiece(uint32_t& piece, SDL_Rect& tile);
    static void MovePieceToMouse(uint32_t& piece);
    static void PlacePiece(uint32_t& piece, Vector2D& position);

    static uint32_t** SaveState();
    static void CopyState(uint32_t** state);
    static void DestroyCopy(uint32_t** copiedBoard);

    void DrawBoard();

    void DrawPossibleMoves(std::vector<Vector2D>& moves);
    void DrawAttackedTiles();



private:

    void FENParser(const char* FEN, Player*& player, Player*& enemy);
    
};