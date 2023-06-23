#pragma once

#include <SDL2/SDL.h>
#include "Vector2D.h"

#include "Helpers/HelperMacros.h"



class BoardPiece {

public:
    BoardPiece() 
        : piece(0), pos({0, 0})
    {

    }


    BoardPiece(uint32_t _piece, Vector2D _position) 
        : piece(_piece), pos(_position)
    {

    }

    uint32_t piece;
    Vector2D pos;

    static inline uint8_t getNewUID(uint32_t color) {
        SDL_Log("Current W UID: %d", currentWhiteUID);

        switch(color) {
            case 64:
                return currentWhiteUID ++;

            case 128:
                return currentBlackUID ++;
        }
    }

    static inline uint8_t assignUID(uint32_t& piece, uint8_t UID) {
        piece |= (UID << 24);
        return UID;
    }

    static inline constexpr uint8_t getPieceUID(uint32_t& piece) {
        return ((piece >> 24) & UID_MASK);
    }

    static const char* PrintPieceType(uint32_t piece) {
        if(__CHESS_EMPTY_TILE(piece))
            return " ";

        switch(GetPieceType(piece)) {
            case 32:
                return "PAWN";
            case 16:
                return "ROOK";
            case 8:
                return "KNIGHT";
            case 4:
                return "BISHOP";
            case 2:
                return "QUEEN";
            case 1:
                return "KING";
            case 0:
                return " ";
            default:
                return " ";
        }
    }


    //  Getters
    static uint32_t GetPieceType(uint32_t& piece);
    static uint32_t GetPieceColor(uint32_t& piece);
    static SDL_Rect GetPiecePosition(uint32_t& piece);
    static SDL_Rect GetMiniPiecePosition(uint32_t& piece);


private:
    //  Will use 8 bits as the UID
    static uint8_t currentWhiteUID;
    static uint8_t currentBlackUID;

};