#include "MoveHandler.h"

#include "Helpers/HelperMacros.h"

namespace Piece = Chess::Pieces;

void MoveHandler::TakePiece(Player*& playerInstance, Player*& other, Vector2D &mousePos)
{
    Board *&boardInstance = Board::getInstance();

    auto boardPiece = __CHESS_GET_PIECE_TILE(boardInstance->CheckForPiece(mousePos));

    if ((!Game::currentTurn && BoardPiece::GetPieceColor(boardPiece) == Piece::WHITE) ||
         (Game::currentTurn && BoardPiece::GetPieceColor(boardPiece) == Piece::BLACK))
    {
        return;
    }

    playerInstance->Piece().piece = boardPiece;
    playerInstance->Piece().pos = mousePos;

    SDL_Log("Player Piece: %d\n", playerInstance->Piece().piece);


    playerInstance->LegalMoves() = MoveHandler::GetLegalPositions(other, playerInstance->Piece().piece, mousePos);

    //  Set the current piece to 0
    Board::board[mousePos.y][mousePos.x] = 0;
}

void MoveHandler::HandleMove(Player*& playerInstance, Player*& enemy, Vector2D &mousePos)
{

    auto &piece = playerInstance->Piece();

    //  Check if position hasn't changed
    if (mousePos == playerInstance->Piece().pos)
    {
        //  Move the piece back
        Board::PlacePiece(
            piece.piece,
            mousePos);

        piece.piece = 0;

        return;
    }

    //  Print the position and the value
    SDL_Log("Player tried to place piece at: {%d, %d} = %d\n", mousePos.x, mousePos.y, Board::board[mousePos.y][mousePos.x]);
    SDL_Log("Is empty? %d\n", __CHESS_EMPTY_TILE(Board::board[mousePos.y][mousePos.x]));
    SDL_Log("Isn't attacked? %d\n", __CHESS_TILE_NOT_ATTACKED(Board::board[mousePos.y][mousePos.x], BoardPiece::GetPieceColor(piece.piece)));
    //  Get temp legal moves
    //  auto tempLegal = GetLegalPositions(piece.piece, mousePos);

    //  Check the moves
    bool legal = MoveHandler::IsLegalMove(
        playerInstance->LegalMoves(),
        mousePos);

    if (legal)
    {
        piece.pos = mousePos;

        //  Get the UID of the piece
        auto pieceID = BoardPiece::getPieceUID(piece.piece);
        SDL_Log("\n\nUID: %d\n\n", pieceID);
        //  Get the player pieces map
        if(Game::currentTurn) {
            playerInstance->PlayerPieces()[pieceID].pos = mousePos;
        } else {
            playerInstance->PlayerPieces()[pieceID].pos = mousePos;
        }
        //  SDL_Log("   =====   MovedPiece: %d  =====\n\n", playerInstance->PlayerPieces()[pieceID].piece);


        //  If the tile is not empty
        if (!__CHESS_EMPTY_TILE(Board::board[mousePos.y][mousePos.x]))
        {
            playerInstance->TakePiece(enemy, Board::board[mousePos.y][mousePos.x]);
            SDL_Log("Taken piece: %d", Board::board[mousePos.y][mousePos.x]);
            SDL_Log("Taken index: %d", playerInstance->TakenIndex());

            //  Play the take sound
            SoundManager::PlaySound(Chess::Sounds::CAPTURE);
        }
        else
        {
            //  Play the normal move sound
            SoundManager::PlaySound(Chess::Sounds::MOVE);
        }

        Board::PlacePiece(
            piece.piece,
            mousePos);


        //  Check if it was a king
        if(BoardPiece::GetPieceType(piece.piece) == Piece::KING) {

            //  Modify the position
            switch(BoardPiece::GetPieceColor(piece.piece)) {
                case Piece::WHITE:
                    CheckingService::wKPos = mousePos;
                    break;
                case Piece::BLACK:
                    CheckingService::bKPos = mousePos;
                    break;
            }


        }

        //  Set the turn to black
        Game::currentTurn = !Game::currentTurn;
    }
    else
    {
        //  Play illegal sound
        SoundManager::PlaySound(Chess::Sounds::ILLEGAL);

        //  Move the piece back
        Board::PlacePiece(
            piece.piece,
            piece.pos);
    }

    //  The piece has been placed, somewhere
    playerInstance->Piece().piece = 0;
}


//  @debug
void MoveHandler::PrintBoard() {

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            printf("%s\t", BoardPiece::PrintPieceType(Board::board[i][j]));
        }
        printf("\n");
    }


}



std::vector<Vector2D> MoveHandler::GetLegalPositions(Player*& other, uint32_t &piece, Vector2D &position)
{
    std::vector<Vector2D> possibleMoves;

    //  Get the positions based on the piece type
    switch (BoardPiece::GetPieceType(piece))
    {

    case Piece::KNIGHT:
    {

        HandleKnight(other, piece, position, possibleMoves);

        break;
    }

    case Piece::BISHOP:
    {
        HandleBishop(other, piece, position, possibleMoves);

        break;
    }

    case Piece::ROOK:
    {
        HandleRook(other, piece, position, possibleMoves);

        break;
    }

    case Piece::KING:
    {

        HandleKing(other, piece, position, possibleMoves);

        break;
    }

    case Piece::QUEEN:
    {
        HandleQueen(other, piece, position, possibleMoves);
        break;
    }

    case Piece::PAWN:
    {
        HandlePawn(other, piece, position, possibleMoves);

        break;
    }
    }

    return possibleMoves;
}

void MoveHandler::HandlePawn(Player*& other, uint32_t &piece, Vector2D &position, std::vector<Vector2D> &currentMoves)
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
            //  If next tile is empty
            if (__CHESS_EMPTY_TILE(Board::board[y - 1][x]))
            {
                Vector2D move;

                move.x = x;
                move.y = y - 1; 

                bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

                if(!isStillChecked) currentMoves.push_back(move);
            }
            //  If there is corner right piece
            if (x + 1 < 8 && !__CHESS_EMPTY_TILE(Board::board[y - 1][x + 1]) && __CHESS_NOT_COLOR(Board::board[y - 1][x + 1], Piece::WHITE))
            {
                Vector2D move;

                move.x = x + 1;
                move.y = y - 1;

                bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

                if(!isStillChecked) currentMoves.push_back(move);
            }
            //  If there is corner left piece
            if (x - 1 >= 0 && !__CHESS_EMPTY_TILE(Board::board[y - 1][x - 1]) && __CHESS_NOT_COLOR(Board::board[y - 1][x - 1], Piece::WHITE))
            {
                Vector2D move;

                move.x = x - 1;
                move.y = y - 1;

                bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

                if(!isStillChecked) currentMoves.push_back(move);
            }
        }
        //  If it's on the next to last row
        if (y == 6 && __CHESS_EMPTY_TILE(Board::board[y - 2][x]) && __CHESS_EMPTY_TILE(Board::board[y - 1][x]))
        {
            Vector2D move;

            move.y = y - 2;
            move.x = x;

            bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

            if(!isStillChecked) currentMoves.push_back(move);
        }
        break;
    }
    case Piece::PieceColors::BLACK:
    {
        if (y + 1 < 8)
        {
            if (__CHESS_EMPTY_TILE(Board::board[y + 1][x]))
            {
                Vector2D move;

                move.x = x;
                move.y = y + 1;

                bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

                if(!isStillChecked) currentMoves.push_back(move);
            }
            //  If there is corner right piece
            if (x + 1 < 8 && !__CHESS_EMPTY_TILE(Board::board[y + 1][x + 1]) && __CHESS_NOT_COLOR(Board::board[y + 1][x + 1], Piece::BLACK))
            {
                Vector2D move;

                move.x = x + 1;
                move.y = y + 1;

                bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

                if(!isStillChecked) currentMoves.push_back(move);
            }
            //  If there is corner left piece
            if (x - 1 >= 0 && !__CHESS_EMPTY_TILE(Board::board[y + 1][x - 1]) && __CHESS_NOT_COLOR(Board::board[y + 1][x - 1], Piece::BLACK))
            {
                Vector2D move;

                move.x = x - 1;
                move.y = y + 1;

                bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

                if(!isStillChecked) currentMoves.push_back(move);
            }
        }
        //  If it's on the second row
        if (y == 1 && __CHESS_EMPTY_TILE(Board::board[y + 2][x]) && __CHESS_EMPTY_TILE(Board::board[y + 1][x]))
        {
            Vector2D move;

            move.y = y + 2;
            move.x = x;

            bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

            if(!isStillChecked) currentMoves.push_back(move);
        }
        break;
    }
    }
}
void MoveHandler::HandleKnight(Player*& other, uint32_t &piece, Vector2D &position, std::vector<Vector2D> &currentMoves)
{
    //  X y aliases
    int &x = position.x;
    int &y = position.y;

    uint32_t color = BoardPiece::GetPieceColor(piece);

    //  Handle top
    if (y - 2 >= 0)
    {
        if (x - 1 >= 0 && __CHESS_NOT_COLOR(Board::board[y - 2][x - 1], color))
        {
            Vector2D move;

            move.y = y - 2;
            move.x = x - 1;

            bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

            if(!isStillChecked) currentMoves.push_back(move);
        }

        if (x + 1 < 8 && __CHESS_NOT_COLOR(Board::board[y - 2][x + 1], color))
        {
            Vector2D move;

            move.y = y - 2;
            move.x = x + 1;

            bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

            if(!isStillChecked) currentMoves.push_back(move);
        }
    }

    //  Handle the bottom
    if (y + 2 < 8)
    {
        if (x - 1 >= 0 && __CHESS_NOT_COLOR(Board::board[y + 2][x - 1], color))
        {
            Vector2D move;

            move.y = y + 2;
            move.x = x - 1;

            bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

            if(!isStillChecked) currentMoves.push_back(move);
        }

        if (x + 1 < 8 && __CHESS_NOT_COLOR(Board::board[y + 2][x + 1], color))
        {
            Vector2D move;

            move.y = y + 2;
            move.x = x + 1;

            bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

            if(!isStillChecked) currentMoves.push_back(move);
        }
    }

    //  Handle the right
    if (x + 2 < 8)
    {
        if (y - 1 >= 0 && __CHESS_NOT_COLOR(Board::board[y - 1][x + 2], color))
        {
            Vector2D move;

            move.y = y - 1;
            move.x = x + 2;

            bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

            if(!isStillChecked) currentMoves.push_back(move);
        }

        if (y + 1 < 8 && __CHESS_NOT_COLOR(Board::board[y + 1][x + 2], color))
        {
            Vector2D move;

            move.y = y + 1;
            move.x = x + 2;

            bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

            if(!isStillChecked) currentMoves.push_back(move);
        }
    }

    //  Handle the left
    if (x - 2 >= 0)
    {
        if (y - 1 >= 0 && __CHESS_NOT_COLOR(Board::board[y - 1][x - 2], color))
        {
            Vector2D move;

            move.y = y - 1;
            move.x = x - 2;

            bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

            if(!isStillChecked) currentMoves.push_back(move);
        }

        if (y + 1 < 8 && __CHESS_NOT_COLOR(Board::board[y + 1][x - 2], color))
        {
            Vector2D move;

            move.y = y + 1;
            move.x = x - 2;

            bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

            if(!isStillChecked) currentMoves.push_back(move);
        }
    }
}
void MoveHandler::HandleBishop(Player*& other, uint32_t &piece, Vector2D &position, std::vector<Vector2D> &currentMoves)
{

    int &x = position.x;
    int &y = position.y;

    uint32_t color = BoardPiece::GetPieceColor(piece);

    Vector2D move;

    //  Alias for Board::board
    auto &board = Board::board;

    //  Handle up - left
    for (int step = 1; step < 8 && y - step >= 0 && x - step >= 0; step++)
    {
        move.y = y - step;
        move.x = x - step;

        //  Log the current possible move
        //  SDL_Log("Current possible BISHOP move: {%d, %d}\n", move.x, move.y);

        //  If overflow
        //  if(move.y < 0 || move.x < 0) break;

        //  If it finds a piece
        if (!__CHESS_EMPTY_TILE(board[move.y][move.x]))
        {
            //  If it hasn't the same color
            if (__CHESS_NOT_COLOR(board[move.y][move.x], color))
            {
                bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

                if(!isStillChecked) currentMoves.push_back(move);
            }

            break;
        }

        bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

        if(!isStillChecked) currentMoves.push_back(move);
    }

    //  Handle up - right
    for (int step = 1; step < 8 && y - step >= 0 && x + step < 8; step++)
    {
        move.y = y - step;
        move.x = x + step;

        //  Log the current possible move
        //  SDL_Log("Current possible BISHOP move: {%d, %d}\n", move.x, move.y);

        //  If overflow
        //  if(move.y < 0 || move.x >= 8) break;

        //  If it finds a piece
        if (!__CHESS_EMPTY_TILE(board[move.y][move.x]))
        {
            //  If it hasn't the same color
            if (__CHESS_NOT_COLOR(board[move.y][move.x], color))
            {
                bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

                if(!isStillChecked) currentMoves.push_back(move);
            }

            break;
        }

        bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

        if(!isStillChecked) currentMoves.push_back(move);
    }

    //  Handle down - left
    for (int step = 1; step < 8 && y + step < 8 && x - step >= 0; step++)
    {
        move.y = y + step;
        move.x = x - step;

        //  Log the current possible move
        //  SDL_Log("Current possible BISHOP move: {%d, %d}\n", move.x, move.y);

        //  If overflow
        //  if(move.y >= 8 || move.x < 0) break;

        //  If it finds a piece
        if (!__CHESS_EMPTY_TILE(board[move.y][move.x]))
        {
            //  If it hasn't the same color
            if (__CHESS_NOT_COLOR(board[move.y][move.x], color))
            {
                bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

                if(!isStillChecked) currentMoves.push_back(move);
            }

            break;
        }

        bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

        if(!isStillChecked) currentMoves.push_back(move);
    }

    //  Handle down - right
    for (int step = 1; step < 8 && y + step < 8 && x + step < 8; step++)
    {
        move.y = y + step;
        move.x = x + step;

        //  Log the current possible move
        //  SDL_Log("Current possible BISHOP move: {%d, %d}\n", move.x, move.y);

        //  If overflow
        //  if(move.y >= 8 || move.x >= 8) break;

        //  If it finds a piece
        if (!__CHESS_EMPTY_TILE(board[move.y][move.x]))
        {
            //  If it hasn't the same color
            if (__CHESS_NOT_COLOR(board[move.y][move.x], color))
            {
                bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

                if(!isStillChecked) currentMoves.push_back(move);
            }

            break;
        }

        bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

        if(!isStillChecked) currentMoves.push_back(move);
    }
}
void MoveHandler::HandleRook(Player*& other, uint32_t &piece, Vector2D &position, std::vector<Vector2D> &currentMoves)
{
    int &x = position.x;
    int &y = position.y;

    auto color = BoardPiece::GetPieceColor(piece);
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
            //  Check if the piece doesn't have the same color
            if (__CHESS_NOT_COLOR(board[move.y][move.x], color))
            {
                bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

                if(!isStillChecked) currentMoves.push_back(move);
            }

            break;
        }

        bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

        if(!isStillChecked) currentMoves.push_back(move);
    }

    //  Handle horiz-right
    for (int step = 1; step < 8 && x + step < 8; step++)
    {
        move.x = x + step;
        move.y = y;

        //  if(move.x < 0) break;

        if (!__CHESS_EMPTY_TILE(board[move.y][move.x]))
        {
            //  Check if the piece doesn't have the same color
            if (__CHESS_NOT_COLOR(board[move.y][move.x], color))
            {
                bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

                if(!isStillChecked) currentMoves.push_back(move);
            }

            break;
        }

        bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

        if(!isStillChecked) currentMoves.push_back(move);
    }

    //  Handle up
    for (int step = 1; step < 8 && y - step >= 0; step++)
    {
        move.x = x;
        move.y = y - step;

        //  if(move.x < 0) break;

        if (!__CHESS_EMPTY_TILE(board[move.y][move.x]))
        {
            //  Check if the piece doesn't have the same color
            if (__CHESS_NOT_COLOR(board[move.y][move.x], color))
            {
                bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

                if(!isStillChecked) currentMoves.push_back(move);
            }

            break;
        }

        bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

        if(!isStillChecked) currentMoves.push_back(move);
    }

    //  Handle down
    for (int step = 1; step < 8 && y + step < 8; step++)
    {
        move.x = x;
        move.y = y + step;

        //  if(move.x < 0) break;

        if (!__CHESS_EMPTY_TILE(board[move.y][move.x]))
        {
            //  Check if the piece doesn't have the same color
            if (__CHESS_NOT_COLOR(board[move.y][move.x], color))
            {
                bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

                if(!isStillChecked) currentMoves.push_back(move);
            }

            break;
        }

        bool isStillChecked = CheckingService::WouldBeCheck(other, Game::currentTurn ? CheckingService::isWhiteKingChecked : CheckingService::isBlackKingChecked, piece, position, move);

                if(!isStillChecked) currentMoves.push_back(move);
    }
}

void MoveHandler::HandleKing(Player*& other, uint32_t &piece, Vector2D &position, std::vector<Vector2D> &currentMoves)
{

    int &x = position.x;
    int &y = position.y;

    auto color = BoardPiece::GetPieceColor(piece);
    auto &board = Board::board;

    Vector2D move;

    if (x - 1 >= 0 && (__CHESS_NOT_COLOR(board[y][x - 1], color) || __CHESS_EMPTY_TILE(board[y][x - 1])) && __CHESS_TILE_NOT_ATTACKED(board[y][x - 1], color))
    {

        move.x = x - 1;
        move.y = y;

        currentMoves.push_back(move);
    }

    if (x + 1 < 8 &&
        (__CHESS_NOT_COLOR(board[y][x + 1], color) || __CHESS_EMPTY_TILE(board[y][x + 1])) && __CHESS_TILE_NOT_ATTACKED(board[y][x + 1], color))
    {

        move.x = x + 1;
        move.y = y;

        currentMoves.push_back(move);
    }

    if (x - 1 >= 0 && y - 1 >= 0 &&
        (__CHESS_EMPTY_TILE(board[y - 1][x - 1]) || __CHESS_NOT_COLOR(board[y - 1][x - 1], color)) && __CHESS_TILE_NOT_ATTACKED(board[y - 1][x - 1], color))
    {
        move.x = x - 1;
        move.y = y - 1;

        currentMoves.push_back(move);
    }

    if (y - 1 >= 0 &&
        (__CHESS_EMPTY_TILE(board[y - 1][x]) || __CHESS_NOT_COLOR(board[y - 1][x], color)) && __CHESS_TILE_NOT_ATTACKED(board[y - 1][x], color))
    {
        move.x = x;
        move.y = y - 1;

        currentMoves.push_back(move);
    }

    if (y - 1 >= 0 && x + 1 < 8 &&
        (__CHESS_EMPTY_TILE(board[y - 1][x + 1]) || __CHESS_NOT_COLOR(board[y - 1][x + 1], color)) && __CHESS_TILE_NOT_ATTACKED(board[y - 1][x + 1], color))
    {
        move.x = x + 1;
        move.y = y - 1;

        currentMoves.push_back(move);
    }

    if (y + 1 < 8 && x - 1 >= 0 &&
        (__CHESS_EMPTY_TILE(board[y + 1][x - 1]) || __CHESS_NOT_COLOR(board[y + 1][x - 1], color)) && __CHESS_TILE_NOT_ATTACKED(board[y + 1][x - 1], color))
    {
        move.x = x - 1;
        move.y = y + 1;

        currentMoves.push_back(move);
    }

    if (y + 1 < 8 &&
        (__CHESS_EMPTY_TILE(board[y + 1][x]) || __CHESS_NOT_COLOR(board[y + 1][x], color)) && __CHESS_TILE_NOT_ATTACKED(board[y + 1][x], color))
    {
        move.x = x;
        move.y = y + 1;

        currentMoves.push_back(move);
    }

    if (y + 1 < 8 && x + 1 < 8 &&
        (__CHESS_EMPTY_TILE(board[y + 1][x + 1]) || __CHESS_NOT_COLOR(board[y + 1][x + 1], color)) && __CHESS_TILE_NOT_ATTACKED(board[y + 1][x + 1], color))
    {
        move.x = x + 1;
        move.y = y + 1;

        currentMoves.push_back(move);
    }
}
void MoveHandler::HandleQueen(Player*& other, uint32_t &piece, Vector2D &position, std::vector<Vector2D> &currentMoves)
{
    HandleBishop(other, piece, position, currentMoves);
    HandleRook(other, piece, position, currentMoves);
}

bool MoveHandler::IsLegalMove(std::vector<Vector2D> &legalMoves, Vector2D &position)
{

    for (auto &move : legalMoves)
    {
        if (move == position)
            return true;
    }

    return false;
}