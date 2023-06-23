#include "FontManager.h"


bool FontManager::Init() {
    //  Init with the default args
    __SDL_ASSERT_FAILURE(
        TTF_Init() == 0,
        "Failed to init fonts: %s\n",
        false
    );

    return true;
}


TTF_Font* FontManager::LoadFont(const char* filePath, int size) {
    
    TTF_Font* font = TTF_OpenFont(filePath, size);

    return font;
}

void FontManager::RenderText(SDL_Texture*& cachedText, int& cachedW, int& cachedH, TTF_Font* font, const char* text, SDL_Color color, SDL_Rect rect) {
    if(cachedText != nullptr) {
            //  Change the w and h
        rect.w = cachedW;
        rect.h = cachedH;

        SDL_RenderCopy(Game::renderer, cachedText, NULL, &rect);

        return;
    }
    
    
    
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);

    SDL_Texture* texture = cachedText = SDL_CreateTextureFromSurface(Game::renderer, surface);

    //  Change the w and h
    rect.w = cachedW = surface->w;
    rect.h = cachedH = surface->h;

    SDL_RenderCopy(Game::renderer, texture, NULL, &rect);
    //  Free the surface
    SDL_FreeSurface(surface);
}