#include "Texture.h"

Texture::Texture(SDL_Texture *sdlTexture)
{
    m_sdlTexture = sdlTexture;
}

Texture::~Texture()
{
    SDL_DestroyTexture(m_sdlTexture);
}

SDL_Texture *Texture::GetSDLTexture()
{
    return m_sdlTexture;
}
