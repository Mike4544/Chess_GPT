#include "CheckingService.h"

#include "Helpers/HelperMacros.h"

namespace Piece = Chess::Pieces;

//  Init the variables
bool CheckingService::isWhiteKingChecked = false;
Vector2D CheckingService::wKPos = {4, 7};
bool CheckingService::whiteKingMoved = false;
uint8_t CheckingService::wKingID = 13;

bool CheckingService::isBlackKingChecked = false;
Vector2D CheckingService::bKPos = {4, 0};
bool CheckingService::blackKingMoved = false;
uint8_t CheckingService::bKingID = 5;




void CheckingService::CheckingCheck() {

    //  Check for white
    if(Game::currentTurn) {

        SDL_Log("White king tile: %d", Board::board[wKPos.y][wKPos.x]);


        if(__CHESS_TILE_ATTACKED(Board::board[wKPos.y][wKPos.x], Chess::Tiles::BLACK_ATTACKED)) {

            isWhiteKingChecked = true;


        } else isWhiteKingChecked = false;

        SDL_Log("W King Check: %d\n", isWhiteKingChecked);

        return;
    }

    SDL_Log("Black king tile: %d", Board::board[bKPos.y][bKPos.x]);

    if(__CHESS_TILE_ATTACKED(Board::board[bKPos.y][bKPos.x], Chess::Tiles::WHITE_ATTACKED)) {
        isBlackKingChecked = true;


    } else isBlackKingChecked = false;

    SDL_Log("B King Check: %d\n", isBlackKingChecked);


}

bool CheckingService::WouldBeCheck(Player*& other, bool prevCheck, uint32_t& piece, Vector2D& prevPos, Vector2D& imaginaryPosition) {
    //  Save the current board
    auto savedState = Board::SaveState();

    Board::board[prevPos.y][prevPos.x] = 0;

    //  Move the pawn to the imaginary position
    Board::board[imaginaryPosition.y][imaginaryPosition.x] = piece;


    //  Play the attacked pieces
    CalculateAttackedTiles(other);
    //  Check the checks
    CheckingCheck();

    // save the result
    bool futureChecked = Game::currentTurn ? isWhiteKingChecked : isBlackKingChecked;

    //  Change the result back;
    if(Game::currentTurn) {
        isWhiteKingChecked = prevCheck;
    } else isBlackKingChecked = prevCheck;


    //  Set the piece back
    Board::CopyState(savedState);

    //  Free the state
    Board::DestroyCopy(savedState);
    

    //  Return the future check
    return futureChecked;
}


bool CheckingService::CheckMateCheck(Player*& currPlayer, Player*& enemy) {

    bool isMate = true;

    //  For every player piece
    auto playerPieces = currPlayer->PlayerPieces();

    for(int i = 1; i < 17; i++) {
        auto& playerPiece = playerPieces[i];

        if(playerPiece.piece == 0) continue;

        //  SDL_Log("Player Piece: %d\n", playerPiece.piece);

        // Get the legal moves
        auto tmpMoves = MoveHandler::GetLegalPositions(enemy, playerPiece.piece, playerPiece.pos);
        SDL_Log("Piece: %s, Legal Moves: %d\n", BoardPiece::PrintPieceType(playerPiece.piece), tmpMoves.size());

        //  If there is a legal move then it is not checkmate
        if(tmpMoves.size() > 0) {
            isMate = false;
            break;
        }
        
    }

    return isMate;



}

void CheckingService::CalculateAttackedTiles(Player*& player)
{

    auto &board = Board::board;

    //  Clear the current attacked tiles
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            //  Clear the attack factor
            __CHESS_CLEAR_ATTACK(board[row][col], Chess::Pieces::BLACK);
            __CHESS_CLEAR_ATTACK(board[row][col], Chess::Pieces::WHITE);
        }
    }

    for(int row = 0; row < 8; row++) {

        for(int col = 0; col < 8; col++) {

            //  If empty continue
            if(__CHESS_EMPTY_TILE(board[row][col])) continue;

            //  If the piece is not the same color continue
            if(BoardPiece::GetPieceColor(board[row][col]) != player->Color()) continue;

            auto& piece = board[row][col];
            Vector2D pos = {col, row};

            //  Get the attack tiles
            auto tmpAttack = GetAttackTiles(piece, pos);

            //  Set the tiles
            SetTiles(piece, tmpAttack, true);


        }

    }

    

    // if(color == Piece::WHITE) {
    //     //  For every player piece
    //     auto playerPieces = player->PlayerPieces();

    //     for(int i = 1; i < 17; i++) {
    //         auto& playerPiece = playerPieces[i];

    //         if(playerPiece.piece == 0) continue;

    //         //  SDL_Log("Player Piece: %d\n", playerPiece.piece);
    //         auto tmpAttack = GetAttackTiles(playerPiece.piece, playerPiece.pos);

    //         SetTiles(playerPiece.piece, tmpAttack, true);
    //     }
    // } else {
    //     //  For every player piece
    //     auto enemyPieces = player->EnemyPieces();

    //     for(int i = 1; i < 17; i++) {
    //         auto& playerPiece = enemyPieces[i];

    //         if(playerPiece.piece == 0) continue;

    //         //  SDL_Log("Player Piece: %d\n", playerPiece.piece);
    //         auto tmpAttack = GetAttackTiles(playerPiece.piece, playerPiece.pos);

    //         SetTiles(playerPiece.piece, tmpAttack, true);
    //     }
    // }
    

}

std::vector<Vector2D> CheckingService::GetAttackTiles(uint32_t &piece, Vector2D &position)
{

    std::vector<Vector2D> attackPos;
    auto type = BoardPiece::GetPieceType(piece);

    switch (type)
    {

    case Piece::PAWN:
    {
        GetAttackPawn(piece, position, attackPos);
        break;
    }
    case Piece::BISHOP:
    {
        GetAttackBishop(position, attackPos);
        break;
    }
    case Piece::KNIGHT:
    {
        GetAttackKnight(position, attackPos);
        break;
    }
    case Piece::ROOK:
    {
        GetAttackRook(position, attackPos);
        break;
    }
    case Piece::KING:
    {
        GetAttackKing(position, attackPos);
        break;
    }
    case Piece::QUEEN:
    {
        GetAttackQueen(position, attackPos);
        break;
    }
    }


    return attackPos;
}


//  Private helpers
void CheckingService::SetTiles(uint32_t &piece, std::vector<Vector2D> &moves, bool attack)
{
    auto color = BoardPiece::GetPieceColor(piece);

    if (attack)
    {
        for (Vector2D &move : moves)
        {
            __CHESS_ATTACK_TILE(Board::board[move.y][move.x], color);
        }
    }
    else
    {
        for (Vector2D &move : moves)
        {
            __CHESS_CLEAR_ATTACK(Board::board[move.y][move.x], color);
        }
    }
}


void CheckingService::GetAttackPawn(uint32_t &piece, Vector2D &position, std::vector<Vector2D> &currentMoves)
{

    //  X y aliases
    int &x = position.x;
    int &y = position.y;

    uint32_t color = BoardPiece::GetPieceColor(piece);

    switch (color)
    {
    case Piece::PieceColors::WHITE:
    {
        //  Handle moves

        //  If there is space
        if (y - 1 >= 0)
        {
            Vector2D move;
            move.y = y - 1;

            if(x - 1 >= 0) {
                move.x = x - 1;
                //  SDL_Log("Legal move for white pawn: {%d, %d}\n", x - 1, y - 1);

                currentMoves.push_back(move);
            }

            if(x + 1 < 8) {
                move.x = x + 1;

                //  SDL_Log("Legal move for white pawn: {%d, %d}\n", x + 1, y - 1);
                currentMoves.push_back(move);
            }
        }
        break;
    }
    case Piece::PieceColors::BLACK:
    {
        if (y + 1 < 8)
        {
            Vector2D move;
            move.y = y + 1;

            if(x - 1 >= 0) {
                move.x = x - 1;

                //  SDL_Log("Legal move for black pawn: {%d, %d}\n", x - 1, y + 1);
                currentMoves.push_back(move);
            }

            if(x + 1 < 8) {
                move.x = x + 1;

                //  SDL_Log("Legal move for black pawn: {%d, %d}\n", x + 1, y + 1);
                currentMoves.push_back(move);
            }
        }

        break;
    }
    }
}
void CheckingService::GetAttackKnight(Vector2D &position, std::vector<Vector2D> &currentMoves) {
     //  X y aliases
    int &x = position.x;
    int &y = position.y;


    //  Handle top
    if (y - 2 >= 0)
    {
        if (x - 1 >= 0)
        {
            Vector2D move;

            move.y = y - 2;
            move.x = x - 1;

            currentMoves.push_back(move);
        }

        if (x + 1 < 8)
        {
            Vector2D move;

            move.y = y - 2;
            move.x = x + 1;

            currentMoves.push_back(move);
        }
    }

    //  Handle the bottom
    if (y + 2 < 8)
    {
        if (x - 1 >= 0 )
        {
            Vector2D move;

            move.y = y + 2;
            move.x = x - 1;

            currentMoves.push_back(move);
        }

        if (x + 1 < 8)
        {
            Vector2D move;

            move.y = y + 2;
            move.x = x + 1;

            currentMoves.push_back(move);
        }
    }

    //  Handle the right
    if (x + 2 < 8)
    {
        if (y - 1 >= 0 )
        {
            Vector2D move;

            move.y = y - 1;
            move.x = x + 2;

            currentMoves.push_back(move);
        }

        if (y + 1 < 8)
        {
            Vector2D move;

            move.y = y + 1;
            move.x = x + 2;

            currentMoves.push_back(move);
        }
    }

    //  Handle the left
    if (x - 2 >= 0)
    {
        if (y - 1 >= 0)
        {
            Vector2D move;

            move.y = y - 1;
            move.x = x - 2;

            currentMoves.push_back(move);
        }

        if (y + 1 < 8)
        {
            Vector2D move;

            move.y = y + 1;
            move.x = x - 2;

            currentMoves.push_back(move);
        }
    }
}
void CheckingService::GetAttackBishop(Vector2D &position, std::vector<Vector2D> &currentMoves) {
    int &x = position.x;
    int &y = position.y;


    Vector2D move;

    //  Alias for Board::board
    auto &board = Board::board;

    //  Handle up - left
    for (int step = 1; step < 8 && y - step >= 0 && x - step >= 0; step++)
    {
        move.y = y - step;
        move.x = x - step;

        //  Log the current possible move
        //  SDL_Log("Current attacked BISHOP move: {%d, %d}\n", move.x, move.y);

        //  If overflow
        //  if(move.y < 0 || move.x < 0) break;

        //  If it finds a piece
        if (!__CHESS_EMPTY_TILE(board[move.y][move.x]))
        {
            currentMoves.push_back(move);

            break;
        }

        currentMoves.push_back(move);
    }

    //  Handle up - right
    for (int step = 1; step < 8 && y - step >= 0 && x + step < 8; step++)
    {
        move.y = y - step;
        move.x = x + step;

        //  Log the current attacked move
        //  SDL_Log("Current attacked BISHOP move: {%d, %d}\n", move.x, move.y);

        //  If overflow
        //  if(move.y < 0 || move.x >= 8) break;

        //  If it finds a piece
        if (!__CHESS_EMPTY_TILE(board[move.y][move.x]))
        {
            currentMoves.push_back(move);

            break;
        }

        currentMoves.push_back(move);
    }

    //  Handle down - left
    for (int step = 1; step < 8 && y + step < 8 && x - step >= 0; step++)
    {
        move.y = y + step;
        move.x = x - step;

        //  Log the current attacked move
        //  SDL_Log("Current attacked BISHOP move: {%d, %d}\n", move.x, move.y);

        //  If overflow
        //  if(move.y >= 8 || move.x < 0) break;

        //  If it finds a piece
        if (!__CHESS_EMPTY_TILE(board[move.y][move.x]))
        {
            currentMoves.push_back(move);

            break;
        }

        currentMoves.push_back(move);
    }

    //  Handle down - right
    for (int step = 1; step < 8 && y + step < 8 && x + step < 8; step++)
    {
        move.y = y + step;
        move.x = x + step;

        //  Log the current attacked move
        //  SDL_Log("Current attacked BISHOP move: {%d, %d}\n", move.x, move.y);

        //  If overflow
        //  if(move.y >= 8 || move.x >= 8) break;

        //  If it finds a piece
        if (!__CHESS_EMPTY_TILE(board[move.y][move.x]))
        {
            currentMoves.push_back(move);

            break;
        }

        currentMoves.push_back(move);
    }
}
void CheckingService::GetAttackRook(Vector2D &position, std::vector<Vector2D> &currentMoves) {
    int &x = position.x;
    int &y = position.y;

    auto &board = Board::board;

    Vector2D move;

    //  Handle horiz-left
    for (int step = 1; step < 8 && x - step >= 0; step++)
    {
        move.x = x - step;
        move.y = y;

        //  if(move.x < 0) break;

        if (!__CHESS_EMPTY_TILE(board[move.y][move.x]))
        {
           currentMoves.push_back(move);

            break;
        }

        currentMoves.push_back(move);
    }

    //  Handle horiz-right
    for (int step = 1; step < 8 && x + step < 8; step++)
    {
        move.x = x + step;
        move.y = y;

        //  if(move.x < 0) break;

        if (!__CHESS_EMPTY_TILE(board[move.y][move.x]))
        {
            currentMoves.push_back(move);

            break;
        }

        currentMoves.push_back(move);
    }

    //  Handle up
    for (int step = 1; step < 8 && y - step >= 0; step++)
    {
        move.x = x;
        move.y = y - step;

        //  if(move.x < 0) break;

        if (!__CHESS_EMPTY_TILE(board[move.y][move.x]))
        {
            currentMoves.push_back(move);

            break;
        }

        currentMoves.push_back(move);
    }

    //  Handle down
    for (int step = 1; step < 8 && y + step < 8; step++)
    {
        move.x = x;
        move.y = y + step;

        //  if(move.x < 0) break;

        if (!__CHESS_EMPTY_TILE(board[move.y][move.x]))
        {
            currentMoves.push_back(move);

            break;
        }

        currentMoves.push_back(move);
    }
}

void CheckingService::GetAttackKing(Vector2D &position, std::vector<Vector2D> &currentMoves) {
    int &x = position.x;
    int &y = position.y;

    Vector2D move;

    if (x - 1 >= 0)
    {

        move.x = x - 1;
        move.y = y;

        currentMoves.push_back(move);
    }

    if (x + 1 < 8)
    {

        move.x = x + 1;
        move.y = y;

        currentMoves.push_back(move);
    }

    if (x - 1 >= 0 && y - 1 >= 0)
    {
        move.x = x - 1;
        move.y = y - 1;

        currentMoves.push_back(move);
    }

    if (y - 1 >= 0)
    {
        move.x = x;
        move.y = y - 1;

        currentMoves.push_back(move);
    }

    if (y - 1 >= 0 && x + 1 < 8)
    {
        move.x = x + 1;
        move.y = y - 1;

        currentMoves.push_back(move);
    }

    if (y + 1 >= 0 && x - 1 >= 0)
    {
        move.x = x - 1;
        move.y = y + 1;

        currentMoves.push_back(move);
    }

    if (y + 1 >= 0)
    {
        move.x = x;
        move.y = y + 1;

        currentMoves.push_back(move);
    }

    if (y + 1 >= 0 && x + 1 >= 0)
    {
        move.x = x + 1;
        move.y = y + 1;

        currentMoves.push_back(move);
    }
}
void CheckingService::GetAttackQueen(Vector2D& position, std::vector<Vector2D>& currentMoves) {

    GetAttackBishop(position, currentMoves);
    GetAttackRook(position, currentMoves);
}

