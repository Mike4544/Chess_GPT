#include "UI.h"

UI::UI(const char* pName) {
    _playerName = pName;

    _font = FontManager::LoadFont(GraphicsInterface::Text::FONT_PATH, 20);
    _miniPiecesSheet = TextureManager::LoadTexture(GraphicsInterface::PlayersWindow::MINI_PIECES_PATH, 96, 32);
}

UI::~UI() {
    // SDL_DestroyTexture(_miniPiecesSheet);
    // SDL_DestroyTexture(_mainMenuBkg);

    SDL_DestroyTexture(_cachedPlayer1Name);
    SDL_DestroyTexture(_cachedPlayer2Name);
    SDL_DestroyTexture(_miniPiecesSheet);
}



void UI::RenderPlayerWindow() {
    SDL_Rect bkgRect = {GraphicsInterface::PlayersWindow::START_X, GraphicsInterface::PlayersWindow::START_Y, 250, 8 * Chess::Tiles::TILE_HEIGHT};
    SDL_SetRenderDrawColor(Game::renderer, GraphicsInterface::PlayersWindow::BKG_TILE.r, GraphicsInterface::PlayersWindow::BKG_TILE.g, GraphicsInterface::PlayersWindow::BKG_TILE.b, GraphicsInterface::PlayersWindow::BKG_TILE.a);
    SDL_RenderFillRect(Game::renderer, &bkgRect);


    //  Render the player name
    SDL_Rect player1NameRect = {
        GraphicsInterface::Text::PLAYER1_NAME_POS.x, 
        GraphicsInterface::Text::PLAYER1_NAME_POS.y, 
        0, 
        0
    };

    SDL_Rect player2NameRect = {
        GraphicsInterface::Text::PLAYER2_NAME_POS.x, 
        GraphicsInterface::Text::PLAYER2_NAME_POS.y, 
        0, 
        0
    };

    //  Render the player name
    FontManager::RenderText(_cachedPlayer1Name, cachedPlayer1W, cachedPlayer1H, _font, _playerName, GraphicsInterface::Text::WHITE, player1NameRect);
    FontManager::RenderText(_cachedPlayer2Name, cachedPlayer2W, cachedPlayer2H, _font, "GPT", GraphicsInterface::Text::WHITE, player2NameRect);
}


void UI::RenderEatenPieces(Player*& player1, Player*& player2) {

    auto p1Pieces = player1->TakenPieces();
    auto p1TakenPiecesNum = player1->TakenIndex();

    auto p2Pieces = player2->TakenPieces();
    auto p2TakenPiecesNum = player2->TakenIndex();

    SDL_Rect p2DstRect = {
        GraphicsInterface::PlayersWindow::PLAYER1_TAKEN_PIECES_POS.x, 
        GraphicsInterface::PlayersWindow::PLAYER1_TAKEN_PIECES_POS.y, 
        GraphicsInterface::PlayersWindow::MINI_PIECE_SIZE, 
        GraphicsInterface::PlayersWindow::MINI_PIECE_SIZE,
    };

    SDL_Rect p1DstRect = {
        GraphicsInterface::PlayersWindow::PLAYER2_TAKEN_PIECES_POS.x, 
        GraphicsInterface::PlayersWindow::PLAYER2_TAKEN_PIECES_POS.y, 
        GraphicsInterface::PlayersWindow::MINI_PIECE_SIZE, 
        GraphicsInterface::PlayersWindow::MINI_PIECE_SIZE,
    };

    for(int i = 0; i < p1TakenPiecesNum; i++) {
        auto srcRect = BoardPiece::GetMiniPiecePosition(p1Pieces[i]);
        TextureManager::Draw(_miniPiecesSheet, srcRect, p1DstRect);

        p1DstRect.x += GraphicsInterface::PlayersWindow::MINI_PIECE_SIZE + 5;
    }

    for(int i = 0; i < p2TakenPiecesNum; i++) {
        auto srcRect = BoardPiece::GetMiniPiecePosition(p2Pieces[i]);
        TextureManager::Draw(_miniPiecesSheet, srcRect, p2DstRect);

        p2DstRect.x += GraphicsInterface::PlayersWindow::MINI_PIECE_SIZE + 5;
    }

}