#include "Game.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    Game* game = new Game();

    //  Simple FPS limiter
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;


    while(game->IsRunning()) {
        //  Get the time at the start of the loop
        uint32_t frameStart = SDL_GetTicks();

        //  Main game loop
        game->HandleEvents();
        game->Update();
        game->Render();

        //  Get the time at the end of the loop
        uint32_t frameTime = SDL_GetTicks() - frameStart;

        //  If the frame took less time than the frameDelay, wait the difference
        if(frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }

    }

    //  Clean up
    delete game;


    return 0;
}