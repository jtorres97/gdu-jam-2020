#include "Renderer.h"

#include <cmath>

const std::string WINDOW_TITLE = "Virusphobia";

static SDL_Rect ConvertRect(Rectangle r, bool flipAndScale = false)
{
    SDL_Rect n = {(int)std::round(r.pos.x),
                  (int)std::round(r.pos.y),
                  (int)std::round(r.size.w),
                  (int)std::round(r.size.h)};

    if (flipAndScale)
    {
        // Flip and scale the position
        n.x *= (RESOLUTION_W / WORLDSIZE_W);
        n.y = (WORLDSIZE_H - n.y - n.h) * (RESOLUTION_H / WORLDSIZE_H);

        // Scale the size
        n.w *= (RESOLUTION_W / WORLDSIZE_W);
        n.h *= (RESOLUTION_H / WORLDSIZE_H);
    }

    return n;
}

// SDLRenderer

void SDLRenderer::Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
    {
        SDL_Log("Failed to initialize SDL: ", std::string(SDL_GetError()));
        exit(1);
    }

    TTF_Init();

    // Create the SDL Window
    m_sdlWindow = SDL_CreateWindow(WINDOW_TITLE.c_str(),
                                   SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   RESOLUTION_W, RESOLUTION_H,
                                   SDL_WINDOW_SHOWN);

    if (m_sdlWindow == NULL)
    {
        SDL_Log("Failed to create SDL window: %s", SDL_GetError());
        exit(1);
    }

    // Create an SDL renderer and attach it to the window
    m_sdlRenderer = SDL_CreateRenderer(m_sdlWindow,
                                       -1,
                                       SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (m_sdlRenderer == NULL)
    {
        SDL_Log("Failed to create SDL renderer: %s", SDL_GetError());
        exit(1);
    }

    // Set render scaling
    // SDL_RenderSetScale(m_sdlRenderer, RENDER_SCALE, RENDER_SCALE);

    // Set the SDL blend mode to BLEND
    if (SDL_SetRenderDrawBlendMode(m_sdlRenderer, SDL_BLENDMODE_BLEND) != 0)
    {
        SDL_Log("Failed to set SDL blend mode: %s", SDL_GetError());
        exit(1);
    }

    // Hide the OS cursor image
    SDL_ShowCursor(SDL_DISABLE);

    // Open the SDL window
    SDL_ShowWindow(m_sdlWindow);
}

void SDLRenderer::Clear()
{
    SDL_SetRenderDrawColor(m_sdlRenderer, BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, BG_COLOR.a);
    SDL_RenderClear(m_sdlRenderer);
}

void SDLRenderer::Present()
{
    SDL_RenderPresent(m_sdlRenderer);
}
void SDLRenderer::Destroy()
{
    SDL_DestroyRenderer(m_sdlRenderer);
    SDL_DestroyWindow(m_sdlWindow);
    SDL_Quit();
}

void SDLRenderer::RenderRectangle(Rectangle rect, int r, int g, int b, int a)
{
    SDL_SetRenderDrawColor(m_sdlRenderer, r, g, b, a);
    SDL_Rect d = ConvertRect(rect, true);
    SDL_RenderFillRect(m_sdlRenderer, &d);
}

void SDLRenderer::RenderWholeTexture(std::shared_ptr<Texture> texture, Rectangle dest)
{
    SDL_Rect d = ConvertRect(dest, true);
    SDL_RenderCopy(m_sdlRenderer, texture->GetSDLTexture(), NULL, &d);
}

void SDLRenderer::RenderWholeTextureRotate(std::shared_ptr<Texture> texture, Rectangle dest, float angle, Point center)
{
    SDL_Rect d = ConvertRect(dest, true);
    SDL_Point c = {(int)center.x, (int)center.y};
    SDL_RenderCopyEx(m_sdlRenderer, texture->GetSDLTexture(), NULL, &d, -angle, &c, SDL_FLIP_NONE);
}

void SDLRenderer::RenderTexture(std::shared_ptr<Texture> texture, Rectangle source, Rectangle dest)
{
    SDL_Rect s = ConvertRect(source);
    SDL_Rect d = ConvertRect(dest, true);
    SDL_RenderCopy(m_sdlRenderer, texture->GetSDLTexture(), &s, &d);
}

void SDLRenderer::RenderTextureRotate(std::shared_ptr<Texture> texture, Rectangle source, Rectangle dest, float angle, Point center)
{
    SDL_Rect s = ConvertRect(source);
    SDL_Rect d = ConvertRect(dest, true);
    SDL_Point c = {(int)center.x, (int)center.y};

    // Note: angle is negative becacause world coordinates are upside down from rendering coordinates
    SDL_RenderCopyEx(m_sdlRenderer, texture->GetSDLTexture(), &s, &d, -angle, &c, SDL_FLIP_NONE);
}

void SDLRenderer::RenderFont(std::shared_ptr<Font> font, const std::string &text, Rectangle dest)
{
    SDL_Surface *s = TTF_RenderText_Blended(font->GetSDLFont(), text.c_str(), FG_COLOR);
    SDL_Texture *t = SDL_CreateTextureFromSurface(m_sdlRenderer, s);

    SDL_Rect r = ConvertRect(dest, true);

    SDL_RenderCopy(m_sdlRenderer, t, NULL, &r);

    SDL_FreeSurface(s);
    SDL_DestroyTexture(t);

    TTF_RenderText_Solid(font->GetSDLFont(), text.c_str(), FG_COLOR);
}

std::shared_ptr<Texture> SDLRenderer::LoadTexture(const std::string &fileName)
{
    return std::make_shared<Texture>(IMG_LoadTexture(m_sdlRenderer, fileName.c_str()));
}

std::shared_ptr<Font> SDLRenderer::LoadFont(const std::string &fileName)
{
    TTF_Font *font = TTF_OpenFont(fileName.c_str(), 80);
    return std::make_shared<Font>(font);
}
