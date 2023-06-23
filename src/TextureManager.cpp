#include "TextureManager.h"

#include "Helpers/HelperMacros.h"


bool TextureManager::Init() {

    //  Init with the default args
    __SDL_ASSERT_FAILURE(
        (IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == IMG_INIT_PNG, 
        "Failed to init images: %s\n",
        false
    );

    return true;

}


SDL_Texture* TextureManager::LoadTexture(const char* filePath, int width, int height) {

    //  Load the surface
    SDL_Surface* tmpSurface = IMG_Load(filePath);

    //  Ensure it is a valid surface
    __SDL_ASSERT_FAILURE(
        tmpSurface != NULL, 
        "Surface failed to initialize: %s\n", 
        ERROR_TEXTURE(width, height)
    );

    //  Instantialize the texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);

    //  Free the surface
    SDL_FreeSurface(tmpSurface);

    return texture;
}



void TextureManager::Draw(SDL_Texture* texture, SDL_Rect src, SDL_Rect dst) {
    SDL_RenderCopy(Game::renderer, texture, &src, &dst);
}

void TextureManager::CleanUp() {
    IMG_Quit();
}