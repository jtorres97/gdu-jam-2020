#include "Sound.h"

Sound::Sound(Mix_Chunk *chunk)
{
    m_chunk = chunk;
}

Mix_Chunk *Sound::GetChunk()
{
    return m_chunk;
}

Music::Music(Mix_Music* music)
{
    m_music = music;
}

Mix_Music* Music::GetMusic()
{
    return m_music;
}