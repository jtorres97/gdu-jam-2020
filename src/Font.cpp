#include "Font.h"

Font::Font(TTF_Font* sdlFont)
{
    m_sdlFont = sdlFont;
}

Font::~Font()
{
    TTF_CloseFont(m_sdlFont);
}

TTF_Font* Font::GetSDLFont()
{
    return m_sdlFont;
}