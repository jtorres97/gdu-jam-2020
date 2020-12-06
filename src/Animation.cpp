#include "Animation.h"
#include "Settings.h"

Animation::Animation() : m_timer(100)
{
}

Animation::Animation(std::shared_ptr<Texture> spriteSheet, int frameCount, int frameWidth, int frameHeight, int frameTime, bool pingPong)
    : m_spriteSheet(spriteSheet), m_frameCount(frameCount), m_frameWidth(frameWidth), m_frameHeight(frameHeight), m_timer(frameTime), m_pingPong(pingPong)
{
    m_center = {0, 0};
    m_rotation = 0.0;
}

void Animation::Update()
{
    if (m_frameCount == 1)
    {
        m_currentFrame = 0;
        return;
    }

    if (m_timer.IsExpired())
    {
        if (m_pingPong)
        {
            if (m_currentFrame == 0)
            {
                m_direction = 1;
            }
            else if (m_currentFrame == m_frameCount - 1)
            {
                m_direction = -1;
            }
        }
        else
        {
            if (m_currentFrame == m_frameCount - 1)
            {
                m_currentFrame = 0;
            }
        }

        m_currentFrame = m_currentFrame + m_direction;
        m_timer.Reset();
    }
}

void Animation::Render(SDLRenderer &renderer, int x, int y)
{
    Rectangle srcRect = {
        0,
        float(m_currentFrame * m_frameHeight),
        float(m_frameWidth),
        float(m_frameHeight)};

    Rectangle destRect = {float(x + m_offSetX * TEXTURE_SCALE),
                          float(y + m_offSetY * TEXTURE_SCALE),
                          float(m_frameWidth * TEXTURE_SCALE),
                          float(m_frameHeight * TEXTURE_SCALE)};

    renderer.RenderTextureRotate(m_spriteSheet, srcRect, destRect, m_rotation * TO_DEGS, m_center);
}

void Animation::SetFrame(int frame)
{
    m_currentFrame = frame;
}

void Animation::SetOffset(int x, int y)
{
    m_offSetX = x;
    m_offSetY = y;
}

void Animation::SetCenter(Point center)
{
    m_center = center;
}

void Animation::SetRotation(double angle)
{
    m_rotation = angle;
}