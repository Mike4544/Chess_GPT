#include "SoundManager.h"

#include "Helpers/HelperMacros.h"


namespace Sounds = Chess::Sounds;

Mix_Chunk* SoundManager::pMove = nullptr;
Mix_Chunk* SoundManager::pCapture = nullptr;
Mix_Chunk* SoundManager::mIllegal = nullptr;
Mix_Chunk* SoundManager::mCheck = nullptr;


bool SoundManager::Init() {

    //  Init with the default args
    __SDL_ASSERT_FAILURE(
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == 0, 
        "Failed to init audio: %s\n",
        false
    );

    //  Init the 2 sounds
    pMove = Mix_LoadWAV(Chess::Sounds::MOVE_SOUND_PATH);
    __SDL_ASSERT_FAILURE(pMove != NULL, "Failed to init move sound: %s\n", false);

    pCapture = Mix_LoadWAV(Chess::Sounds::CAPTURE_SOUND_PATH);
    __SDL_ASSERT_FAILURE(pCapture != NULL, "Failed to init capture sound: %s\n", false);

    mIllegal = Mix_LoadWAV(Chess::Sounds::ILLEGAL_MOVE_PATH);
    __SDL_ASSERT_FAILURE(pCapture != NULL, "Failed to init illegal move sound: %s\n", false);

    mCheck = Mix_LoadWAV(Chess::Sounds::CHECK_SOUND_PATH);
    __SDL_ASSERT_FAILURE(pCapture != NULL, "Failed to init check sound: %s\n", false);

    return true;
}


void SoundManager::PlaySound(Sounds::Sound sound) {

    switch(sound) {
        case Sounds::Sound::MOVE: {
            Mix_PlayChannel(-1, pMove, 0);
            break;
        }

        case Sounds::Sound::CAPTURE: {
            Mix_PlayChannel(-1, pCapture, 0);
            break;
        }

        case Sounds::Sound::ILLEGAL: {
            Mix_PlayChannel(-1, mIllegal, 0);
            break;
        }
        case Sounds::Sound::CHECK: {
            Mix_PlayChannel(-1, mCheck, 0);
            break;
        }

        default:
            break;

    }

}


void SoundManager::CleanUp() {

    Mix_FreeChunk(pMove);
    Mix_FreeChunk(pCapture);
    Mix_FreeChunk(mIllegal);
    Mix_FreeChunk(mCheck);

    pMove = nullptr;
    pCapture = nullptr;
    mIllegal = nullptr;
    mCheck = nullptr;

    Mix_Quit();
}

