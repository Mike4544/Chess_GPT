#pragma once

#include "Game.h"

#include "Vector2D.h"
#include <vector>
#include <stdio.h>


class MoveHandler {

public:
    static void TakePiece(Player*& playerInstance, Player*& other, Vector2D &mousePos);
    static void HandleMove(Player*& playerInstance, Player*& enemy, Vector2D& mousePos);


    static std::vector<Vector2D> GetLegalPositions(Player*& other, uint32_t& piece, Vector2D& position);
    
    static bool IsLegalMove(std::vector<Vector2D>& legalMoves, Vector2D& position);
    static void PrintBoard();

private:
    static void HandlePawn(Player*& other, uint32_t& piece, Vector2D& position, std::vector<Vector2D>& currentMoves);
    static void HandleKnight(Player*& other, uint32_t& piece, Vector2D& position, std::vector<Vector2D>& currentMoves);
    static void HandleBishop(Player*& other, uint32_t& piece, Vector2D& position, std::vector<Vector2D>& currentMoves);
    static void HandleRook(Player*& other, uint32_t& piece, Vector2D& position, std::vector<Vector2D>& currentMoves);

    static void HandleKing(Player*& other, uint32_t& piece, Vector2D& position, std::vector<Vector2D>& currentMoves);
    static void HandleQueen(Player*& other, uint32_t& piece, Vector2D& position, std::vector<Vector2D>& currentMoves);

};