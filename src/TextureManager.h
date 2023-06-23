#pragma once

#include "Game.h"
#include <SDL2/SDL_image.h>


class TextureManager {

public:
    static bool Init();

    static SDL_Texture* LoadTexture(const char* filePath, int width = 64, int height = 64);
    static void Draw(SDL_Texture* texture, SDL_Rect src, SDL_Rect dst);

    //  Clean up
    static void CleanUp();
    
};