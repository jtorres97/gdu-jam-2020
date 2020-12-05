#pragma once

#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Geometry.h"
#include "Texture.h"
#include "Settings.h"

const SDL_Color BG_COLOR = {0x22, 0x20, 0x34, 0xFF};

class SDLRenderer
{
public:
    void Initialize();
    void Clear();
    void Present();
    void Destroy();

    void RenderRectangle(Rectangle rect, int r, int g, int b, int a);
    void RenderWholeTexture(std::shared_ptr<Texture> texture, Rectangle dest);
    void RenderWholeTextureRotate(std::shared_ptr<Texture> texture, Rectangle dest, float angle, Point center);
    void RenderTexture(std::shared_ptr<Texture> texture, Rectangle source, Rectangle dest);
    void RenderTextureRotate(std::shared_ptr<Texture> texture, Rectangle source, Rectangle dest, float angle, Point center);

    std::shared_ptr<Texture> LoadTexture(std::string fileName);

private:
    SDL_Window *m_sdlWindow;
    SDL_Renderer *m_sdlRenderer;
};