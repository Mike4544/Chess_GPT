#pragma once

#include "Game.h"
#include <SDL2/SDL_mixer.h>


class SoundManager {

public:
    static bool Init();
    static void PlaySound(Chess::Sounds::Sound sound);

    //  Cleanup function
    static void CleanUp();

private:
    static Mix_Chunk* pMove;
    static Mix_Chunk* pCapture;
    static Mix_Chunk* mIllegal;
    static Mix_Chunk* mCheck;


};