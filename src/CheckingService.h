#pragma once

#include "Game.h"

#include "Vector2D.h"
#include <vector>

class CheckingService {

public:
    static bool isWhiteKingChecked;
    static Vector2D wKPos;
    static bool whiteKingMoved;
    static uint8_t wKingID;

    static bool isBlackKingChecked;
    static Vector2D bKPos;
    static bool blackKingMoved;
    static uint8_t bKingID;



    static bool WouldBeCheck(Player*& other, bool prevCheck, uint32_t& piece, Vector2D& prevPos, Vector2D& imaginaryPosition);
    static void CheckingCheck();

    static bool CheckMateCheck(Player*& currPlayer, Player*& enemy);


    static void CalculateAttackedTiles(Player*& player);
    static std::vector<Vector2D> GetAttackTiles(uint32_t& piece, Vector2D& position);

private:
    static void SetTiles(uint32_t& piece, std::vector<Vector2D>& legalMoves, bool attack);

    //  Handle the attacked squares
    static void GetAttackPawn(uint32_t& piece, Vector2D& position, std::vector<Vector2D>& currentMoves);
    static void GetAttackKnight(Vector2D& position, std::vector<Vector2D>& currentMoves);
    static void GetAttackBishop(Vector2D& position, std::vector<Vector2D>& currentMoves);
    static void GetAttackRook(Vector2D& position, std::vector<Vector2D>& currentMoves);

    static void GetAttackKing(Vector2D& position, std::vector<Vector2D>& currentMoves);
    static void GetAttackQueen(Vector2D& position, std::vector<Vector2D>& currentMoves);



};