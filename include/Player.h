#pragma once

#include "Entity.h"
#include "Timer.h"
#include "Texture.h"

class Player : public Entity
{
public:
    Player();

    void Update(GameState &state) override;
    void Render(SDLRenderer &renderer) override;

    void SetMainTexture(std::shared_ptr<Texture> mainTexture);

    float GetRotation();
    bool GetFlip();

protected:
    float m_moveSpeed = 1.0f;
    float m_rotation = 0.0f;
    bool m_flip = false;

    Timer m_flipTimer;

    std::shared_ptr<Texture> m_mainTexture;
};