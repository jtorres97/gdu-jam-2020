#pragma once

#include "Entity.h"
#include "Texture.h"

class Enemy : public Entity
{
public:
    Enemy(Point position);

    void Update(GameState& state) override;
    void Render(SDLRenderer& renderer) override;

    void SetMainTexture(std::shared_ptr<Texture> mainTexture);
    
protected:
    std::shared_ptr<Texture> m_mainTexture;
    float m_moveSpeed = 1.0f;
};