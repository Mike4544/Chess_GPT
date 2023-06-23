#pragma once

#include "Game.h"
#include <SDL2/SDL_ttf.h>

class FontManager {

public:
    static bool Init();


    static TTF_Font* LoadFont(const char* filePath, int size);
    static void RenderText(SDL_Texture*& cachedText, int& cachedW, int& cachedH, TTF_Font* font, const char* text, SDL_Color color, SDL_Rect rect);

};