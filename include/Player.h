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

protected:
    float m_rotation = 0.0f;
    std::shared_ptr<Texture> m_mainTexture;
};