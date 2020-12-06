#pragma once

#include <SDL2/SDL_mixer.h>

class Sound
{
public:
    Sound(Mix_Chunk* chunk);

    Mix_Chunk* GetChunk();

private:
    Mix_Chunk* m_chunk;
};

class Music
{
public:
    Music(Mix_Music* music);

    Mix_Music* GetMusic();

private:
    Mix_Music* m_music;
};