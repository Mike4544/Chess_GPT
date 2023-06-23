#pragma once

namespace Chess
{

    constexpr const char* BEGINNING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

    namespace Tiles
    {
        //  Tile sizes
        constexpr int TILE_WIDTH = 64;
        constexpr int TILE_HEIGHT = 64;

        //  Tile colors
        constexpr SDL_Color TILE_BLACK = {
            118, 150, 86, 255};
        constexpr SDL_Color TILE_WHITE = {
            238, 238, 210, 255};

        //  Overlay tile
        constexpr SDL_Color OVERLAY_TILE = {
            250, 237, 92, 122
        };

        //  Attacked tile
        constexpr uint32_t WHITE_ATTACKED = 256;
        constexpr SDL_Color WHITE_ATTACKED_TILE = {
            255, 255, 255, 122
        };

        constexpr uint32_t BLACK_ATTACKED = 512;
        constexpr SDL_Color BLACK_ATTACKED_TILE = {
            0, 0, 0, 122
        };
    }

    namespace Pieces
    {

        //  Piece types
        enum Pieces
        {
            KING = 1,
            QUEEN = 2,
            BISHOP = 4,
            KNIGHT = 8,
            ROOK = 16,
            PAWN = 32
        };

        //  Piece colors
        enum PieceColors
        {
            WHITE = 64,
            BLACK = 128
        };        

    }

    namespace Textures {
        
        constexpr const char* SPRITE_SHEET = "../../res/pieces_sprites.png";

        constexpr int SHEET_WIDTH = 792;
        constexpr int SHEET_HEIGHT = 264;


        constexpr int TILE_WIDTH = SHEET_WIDTH / 6;
        constexpr int TILE_HEIGHT = SHEET_HEIGHT / 2;

    }

    namespace Sounds {

        constexpr const char* MOVE_SOUND_PATH = "../../res/sounds/move.wav";
        constexpr const char* CAPTURE_SOUND_PATH = "../../res/sounds/capture.wav";
        constexpr const char* ILLEGAL_MOVE_PATH = "../../res/sounds/illegal_buzz.wav";
        constexpr const char* CHECK_SOUND_PATH = "../../res/sounds/check.wav";

        enum Sound {
            MOVE,
            CAPTURE,
            ILLEGAL,
            CHECK
        };

    }

} // namespace Chess
