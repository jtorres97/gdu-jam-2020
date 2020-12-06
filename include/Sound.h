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