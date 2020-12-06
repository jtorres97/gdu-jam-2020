#include "Sound.h"

Sound::Sound(Mix_Chunk *chunk)
{
    m_chunk = chunk;
}

Mix_Chunk *Sound::GetChunk()
{
    return m_chunk;
}