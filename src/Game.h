#pragma once

#include <SDL2/SDL.h>

//  Namespace
#include "Chess.h"

//  Subsystems
#include "TextureManager.h"
#include "SoundManager.h"
#include "FontManager.h"

#include "UI.h"


#include "Player.h"
#include "Piece.h"
#include "Board.h"
#include "Vector2D.h"

//  Include the checking service
#include "CheckingService.h"

#include <vector>


class Game{

public:
    //  Constructor
    Game();
    //  Destructor
    ~Game();

    //  Static instance of the renderer
    static SDL_Renderer* renderer;
    //  Static instance of current turn;
    //  True for white
    //  False for black
    static bool currentTurn;


    //  Methods
    void Init(const char* windowTitle, int x, int y, int height, int width, int flags = 0);

    void HandleEvents();
    void Update();
    void Render();

    void CleanUp();

    //  IsRunning getter
    const bool& IsRunning() const { return _isRunning; }

private:
    bool _isRunning = false;
    bool _mouseHasPiece = false;

    SDL_Window* _window = nullptr;

    bool _lastTurn = false;

};