#pragma once

#include <memory>

#include "Renderer.h"
#include "Texture.h"
#include "Timer.h"

class Animation
{
public:
    Animation();
    Animation(std::shared_ptr<Texture> spriteSheet, int frameCount, int frameWidth, int frameHeight, int frameTime, bool pingPong = false);
    void Update();
    void Render(SDLRenderer &renderer, int x, int y);

    void SetFrame(int frame);

    void SetOffset(int x, int y);
    void SetCenter(Point center);
    void SetRotation(double angle);

private:
    std::shared_ptr<Texture> m_spriteSheet = nullptr;
    int m_frameCount = 0;
    int m_frameWidth = 0;
    int m_frameHeight = 0;
    int m_currentFrame = 0;
    int m_direction = 1;
    int m_offSetX = 0;
    int m_offSetY = 0;
    Timer m_timer;
    bool m_pingPong = false;
    Point m_center;
    double m_rotation = 0.0;
};