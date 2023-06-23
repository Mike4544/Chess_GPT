#include "Board.h"

#include "Helpers/HelperMacros.h"

namespace Tile = Chess::Tiles;
namespace Piece = Chess::Pieces;
namespace Texture = Chess::Textures;

uint32_t Board::board[Tile::TILE_HEIGHT][Tile::TILE_WIDTH];
SDL_Texture* Board::spriteSheet = nullptr;

Board::~Board()
{
}

void Board::InitBoard(Player*& player, Player*& enemy)
{

    //  Make the board empty
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            board[i][j] = 0;
        }
    }

    //  Fill with the default pieces
    FENParser(Chess::BEGINNING_FEN, player, enemy);

    //  Init the sprite sheet
    spriteSheet = TextureManager::LoadTexture(
        Texture::SPRITE_SHEET,
        Texture::SHEET_WIDTH,
        Texture::SHEET_HEIGHT);
}


void Board::DrawBoard()
{

    SDL_Rect currentTile = {0, 0, Tile::TILE_WIDTH, Tile::TILE_HEIGHT};

    for (int row = 0; row < 8; row++)
    {

        for (int col = 0; col < 8; col++)
        {

            currentTile.x = col * Tile::TILE_WIDTH;
            currentTile.y = row * Tile::TILE_HEIGHT;

            //  Change the color according to the parity of the tile
            SDL_Color tileColor = ((row + col) & 1) ? Tile::TILE_BLACK : Tile::TILE_WHITE;

            //  Set the renderer draw color
            SDL_SetRenderDrawColor(Game::renderer, tileColor.r, tileColor.g, tileColor.b, tileColor.a);

            //  Draw the rect
            SDL_RenderFillRect(Game::renderer, &currentTile);

            //  Check if there's empty space
            if (!__CHESS_EMPTY_TILE(board[row][col]))
            {
                //  Draw the piece
                DrawPiece(board[row][col], currentTile);
        
            }
        }
    }
}


void Board::DrawPossibleMoves(std::vector<Vector2D>& moves) {
    

    //  Clear the renderer
    //  SDL_RenderClear(Game::renderer);

    //  Set the blend mode
    SDL_SetRenderDrawBlendMode(Game::renderer, SDL_BLENDMODE_ADD);

    //  Set the new color
    SDL_SetRenderDrawColor(
        Game::renderer, 
        Tile::OVERLAY_TILE.r, 
        Tile::OVERLAY_TILE.g, 
        Tile::OVERLAY_TILE.b, 
        Tile::OVERLAY_TILE.a
    );

    //  Make an sdl rect
    SDL_Rect currentTile = {0, 0, Tile::TILE_WIDTH, Tile::TILE_HEIGHT};

    //  Draw the overlay for the moves
    for(auto& move : moves) {

        currentTile.x = move.x * Tile::TILE_WIDTH;
        currentTile.y = move.y * Tile::TILE_HEIGHT;


        SDL_RenderFillRect(Game::renderer, &currentTile);
    }

    SDL_SetRenderDrawBlendMode(Game::renderer, SDL_BLENDMODE_NONE);


}

void Board::DrawAttackedTiles() {

    SDL_Rect currentTile = {0, 0, Tile::TILE_WIDTH, Tile::TILE_HEIGHT};

    SDL_SetRenderDrawBlendMode(Game::renderer, SDL_BLENDMODE_BLEND);


    for(int row = 0; row < 8; row++) {

        for(int col = 0; col < 8; col++) {

            currentTile.x = col * Tile::TILE_WIDTH;
            currentTile.y = row * Tile::TILE_HEIGHT;

            //  Check if white attacks the tile
            if(__CHESS_TILE_ATTACKED(board[row][col], Chess::Tiles::WHITE_ATTACKED)) {
                //  Set the blend mode
                SDL_SetRenderDrawColor(
                    Game::renderer, 
                    Tile::WHITE_ATTACKED_TILE.r, 
                    Tile::WHITE_ATTACKED_TILE.g, 
                    Tile::WHITE_ATTACKED_TILE.b, 
                    Tile::WHITE_ATTACKED_TILE.a
                );

                //  Draw the rect
                SDL_RenderFillRect(Game::renderer, &currentTile);

            }

            //  Check if white attacks the tile
            if(__CHESS_TILE_ATTACKED(board[row][col], Chess::Tiles::BLACK_ATTACKED)) {

                SDL_SetRenderDrawColor(
                    Game::renderer, 
                    Tile::BLACK_ATTACKED_TILE.r, 
                    Tile::BLACK_ATTACKED_TILE.g, 
                    Tile::BLACK_ATTACKED_TILE.b, 
                    Tile::BLACK_ATTACKED_TILE.a
                );

                //  Draw the rect
                SDL_RenderFillRect(Game::renderer, &currentTile);


            }


        }

    }

    SDL_SetRenderDrawBlendMode(Game::renderer, SDL_BLENDMODE_NONE);


}


uint32_t& Board::CheckForPiece(Vector2D& position) {
    //  Edge case if tile is empty
    return board[position.y][position.x];

}

void Board::DrawPiece(uint32_t& piece, SDL_Rect& tile) {
    //  Get the piece coordonates in the sheet
    SDL_Rect piecePosition = BoardPiece::GetPiecePosition(piece);

    //  Draw the piece
    TextureManager::Draw(Board::spriteSheet, piecePosition, tile);
}

void Board::MovePieceToMouse(uint32_t& piece) {
     //  Get the current mouse position
        SDL_Rect mousePos;

        mousePos.w = Chess::Tiles::TILE_WIDTH;
        mousePos.h = Chess::Tiles::TILE_HEIGHT;

        SDL_GetMouseState(&mousePos.x, &mousePos.y);

        mousePos.x -= mousePos.w / 2;
        mousePos.y -= mousePos.h / 2;

        //  SDL_Log("Mouse moved piece to: (%d, %d)", mousePos.x, mousePos.y);

        //  Draw the piece
        Board::DrawPiece(piece, mousePos);
}

void Board::PlacePiece(uint32_t& piece, Vector2D& position) {
    
    //  Get the tile position
    SDL_Rect tile = {
        position.x * Chess::Tiles::TILE_WIDTH,
        position.y * Chess::Tiles::TILE_HEIGHT,
        Chess::Tiles::TILE_WIDTH,
        Chess::Tiles::TILE_HEIGHT
    };

    //  Set the board
    Board::board[position.y][position.x] = piece;

    //  Draw the piece
    Board::DrawPiece(piece, tile);
}


uint32_t** Board::SaveState() {
    uint32_t** newBoard = new uint32_t*[8]();

    for(int i = 0; i < 8; i++) {
        newBoard[i] = new uint32_t[8]();

        for(int j = 0; j < 8; j++) {
            newBoard[i][j] = board[i][j];
        }
    }

    return newBoard;
}

void Board::CopyState(uint32_t** state) {

    for(int i = 0; i < 8; i++) {

        for(int j = 0; j < 8; j++) {
            board[i][j] = state[i][j];
        }
    }
}

void Board::DestroyCopy(uint32_t** copiedBoard) {
    for(int i = 0; i < 8; i++) {

        delete [] copiedBoard[i];

    }

    delete [] copiedBoard;
}




void Board::FENParser(const char *FEN, Player*& player, Player*& enemy)
{

    //  Dictionary for the pieces
    std::unordered_map<char, uint32_t> dictionary = {
        {'r', Piece::BLACK | Piece::ROOK},
        {'n', Piece::BLACK | Piece::KNIGHT},
        {'b', Piece::BLACK | Piece::BISHOP},
        {'q', Piece::BLACK | Piece::QUEEN},
        {'k', Piece::BLACK | Piece::KING},
        {'p', Piece::BLACK | Piece::PAWN},

        {'R', Piece::WHITE | Piece::ROOK},
        {'N', Piece::WHITE | Piece::KNIGHT},
        {'B', Piece::WHITE | Piece::BISHOP},
        {'Q', Piece::WHITE | Piece::QUEEN},
        {'K', Piece::WHITE | Piece::KING},
        {'P', Piece::WHITE | Piece::PAWN},
    };

    int currY = 0, currX = 0;

    for (int i = 0; FEN[i]; i++)
    {

        if (FEN[i] == '/')
        {
            currX = 0;
            ++currY;
            continue;
        }

        if (FEN[i] >= '0' && FEN[i] <= '9')
        {
            currX += FEN[i] - '0';
            continue;
        }

        //  Mark the current piece on the board
        BoardPiece tablePiece;

        tablePiece.piece = dictionary[FEN[i]];
        tablePiece.pos = {currX, currY};
        auto pieceColor = BoardPiece::GetPieceColor(tablePiece.piece);
         //  Assign the UID (or PID)
        auto UID = BoardPiece::assignUID(tablePiece.piece, BoardPiece::getNewUID(pieceColor));

        //  Assign the ID-ed piece to the board pos
        board[currY][currX] = tablePiece.piece;

        //  Pass the piece to the playerPieces vector
        if(BoardPiece::GetPieceColor(board[currY][currX]) == Piece::WHITE) {

            player->PlayerPieces()[UID] = tablePiece;

            SDL_Log("VAR UID: %d \t GOT UID: %d", UID, BoardPiece::getPieceUID(tablePiece.piece));
            
        } else {

            //  Push it to the GPT
            enemy->PlayerPieces()[UID] = tablePiece;

        }


        ++currX;
    }
}