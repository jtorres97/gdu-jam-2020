#pragma once

#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "Geometry.h"
#include "Texture.h"
#include "Settings.h"
#include "Font.h"
#include "Sound.h"

const SDL_Color BG_COLOR = {0x75, 0x3D, 0x2B, 0xFF};
const SDL_Color FG_COLOR = {0xBA, 0x5A, 0x3A, 0xFF};

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
    void RenderFont(std::shared_ptr<Font> font, const std::string& text, Rectangle dest, SDL_Color color);

    std::shared_ptr<Texture> LoadTexture(const std::string& fileName);
    std::shared_ptr<Font> LoadFont(const std::string& fileName);
    std::shared_ptr<Sound> LoadSFX(const std::string& fileName);
    std::shared_ptr<Music> LoadMusic(const std::string& fileName);

    void PlaySFX(std::shared_ptr<Sound> sound);
    void PlayMusic(std::shared_ptr<Music> music);

private:
    SDL_Window *m_sdlWindow;
    SDL_Renderer *m_sdlRenderer;
};
