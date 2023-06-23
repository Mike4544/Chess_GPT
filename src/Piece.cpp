#include "Piece.h"

#include "Chess.h"

namespace Tile = Chess::Tiles;
namespace Piece = Chess::Pieces;
namespace Texture = Chess::Textures;

uint8_t BoardPiece::currentWhiteUID = 1;
uint8_t BoardPiece::currentBlackUID = 1;


SDL_Rect BoardPiece::GetPiecePosition(uint32_t &piece)
{

    //  The default is KING (0, 0)
    int sheetX = 0, sheetY = 0;

    //  Check for color
    sheetY = (GetPieceColor(piece) == Piece::BLACK) * Texture::TILE_HEIGHT;

    //  Get the piece type
    switch(GetPieceType(piece)) {
        case Piece::KING:
            sheetX = 0 * Texture::TILE_WIDTH;
            break;
        case Piece::QUEEN:
            sheetX = 1 * Texture::TILE_WIDTH;
            break;
        case Piece::BISHOP:
            sheetX = 2 * Texture::TILE_WIDTH;
            break;
        case Piece::KNIGHT:
            sheetX = 3 * Texture::TILE_WIDTH;
            break;
        case Piece::ROOK:
            sheetX = 4 * Texture::TILE_WIDTH;
            break;
        case Piece::PAWN:
            sheetX = 5 * Texture::TILE_WIDTH;
            break;
    }

    SDL_Rect piecePosition = {
        sheetX, sheetY, Texture::TILE_WIDTH, Texture::TILE_HEIGHT
    };

    return piecePosition;

}

SDL_Rect BoardPiece::GetMiniPiecePosition(uint32_t &piece)
{

    constexpr int MINI_TILE_SIZE = 16;

    //  The default is KING (0, 0)
    int sheetX = 0, sheetY = 0;

    //  Check for color
    if (GetPieceColor(piece) == Piece::BLACK)
    {
        sheetY = 1 * MINI_TILE_SIZE;
    }

    //  Get the piece type
    switch(GetPieceType(piece)) {
        case Piece::KING:
            sheetX = 0 * MINI_TILE_SIZE;
            break;
        case Piece::QUEEN:
            sheetX = 1 * MINI_TILE_SIZE;
            break;
        case Piece::BISHOP:
            sheetX = 2 * MINI_TILE_SIZE;
            break;
        case Piece::KNIGHT:
            sheetX = 3 * MINI_TILE_SIZE;
            break;
        case Piece::ROOK:
            sheetX = 4 * MINI_TILE_SIZE;
            break;
        case Piece::PAWN:
            sheetX = 5 * MINI_TILE_SIZE;
            break;
    }

    SDL_Rect piecePosition = {
        sheetX, sheetY, MINI_TILE_SIZE, MINI_TILE_SIZE
    };

    return piecePosition;

}

uint32_t BoardPiece::GetPieceType(uint32_t& piece) {
    //  Tests by bitshifting
    for(int i = 0; i <= 5; i++) {
        if((piece & (1 << i))) return (1 << i);
    }

    return (1 << 5);
}

uint32_t BoardPiece::GetPieceColor(uint32_t& piece) {
    if((piece & Piece::BLACK)) return Piece::BLACK;

    return Piece::WHITE;
}
