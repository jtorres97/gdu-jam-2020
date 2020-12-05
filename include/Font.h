#pragma once

#include <memory>

#include <SDL2/SDL_ttf.h>

class Font
{
public:
    Font(TTF_Font* sdlFont);
    ~Font();

    TTF_Font* GetSDLFont();
    
private:
    TTF_Font* m_sdlFont;
};