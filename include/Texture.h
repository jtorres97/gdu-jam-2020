#pragma once

#include <SDL2/SDL.h>

class Texture
{
public:
    Texture(SDL_Texture *sdlTexture);
    ~Texture();

    SDL_Texture *GetSDLTexture();

private:
    SDL_Texture *m_sdlTexture;
};
