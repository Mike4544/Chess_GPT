#pragma once

#include "Game.h"

#include <SDL2/SDL_ttf.h>
#include "FontManager.h"

#include "Vector2D.h"
#include "Player.h"

namespace GraphicsInterface {

    namespace PlayersWindow {

        constexpr int START_X = 8 * Chess::Tiles::TILE_WIDTH;
        constexpr int START_Y = 0;

        constexpr SDL_Color BKG_TILE = {128, 128, 128, 255};

        constexpr const char* MINI_PIECES_PATH = "../../res/mini-pieces.png";


        //  Taken pieces positions
        constexpr Vector2D PLAYER1_TAKEN_PIECES_POS = { 8 * Chess::Tiles::TILE_WIDTH + 10, 25 };
        constexpr Vector2D PLAYER2_TAKEN_PIECES_POS = { 8 * Chess::Tiles::TILE_WIDTH + 10, 8 * Chess::Tiles::TILE_HEIGHT - 75 };

        constexpr int MINI_PIECE_SIZE = 16;
    
    };

    namespace Text {

        constexpr SDL_Color WHITE = {255, 255, 255, 255};
        constexpr SDL_Color BLACK = {0, 0, 0, 255};

        constexpr const char* FONT_PATH = "../../res/fonts/moreSugar.ttf";


        const Vector2D PLAYER2_NAME_POS = { 8 * Chess::Tiles::TILE_WIDTH + 10, 50 };
        const Vector2D PLAYER1_NAME_POS = { 8 * Chess::Tiles::TILE_WIDTH + 10, 8 * Chess::Tiles::TILE_HEIGHT - 50 };


    };


};


class UI {

public:
    UI(const char* pName);

    ~UI();

    void RenderPlayerWindow();

    void RenderEatenPieces(Player*& player1, Player*& player2);




private:
    TTF_Font* _font;
    SDL_Texture* _cachedPlayer1Name = nullptr, *_cachedPlayer2Name = nullptr;
    int cachedPlayer1W, cachedPlayer1H, cachedPlayer2W, cachedPlayer2H;

    SDL_Texture* _miniPiecesSheet, *_mainMenuBkg;
    const char* _playerName;

    

};