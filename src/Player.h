#pragma once

#include "Game.h"

#include "Piece.h"
#include <vector>


class Player {

public:
    Player(uint32_t _color) : color(_color)
    {
        SDL_Log("Creating player...\n");
    }



    inline void TakePiece(Player*& enemy, uint32_t& piece)
    { 
        
            takenPieces[currentTakenIndex ++] = piece;

            auto pieces = enemy->PlayerPieces();

            auto pieceUID = BoardPiece::getPieceUID(piece);

            pieces[pieceUID].piece = 0;
            pieces[pieceUID].pos = {-1, -1};

    }



    inline std::vector<Vector2D>& LegalMoves()
    { return playerLegalMoves; } 

    inline BoardPiece& Piece()
    { return tempPiece; }

    inline BoardPiece* PlayerPieces()
    { return playerPieces; }

    inline uint32_t& Color()
    { return color; }



    inline uint32_t* TakenPieces()
    { return takenPieces; }

    inline uint32_t& TakenIndex()
    { return currentTakenIndex; }

    
private:
    bool isInCheck = false;

    uint32_t color;

    BoardPiece tempPiece;

    BoardPiece playerPieces[17] = {};

    std::vector<Vector2D> playerLegalMoves = {};

    //  An array with possible pieces
    //  There are a total of 16 pieces to take
    uint32_t takenPieces[16] = {};
    uint32_t currentTakenIndex = 0;

};