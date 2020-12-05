#pragma once

#include "Entity.h"
#include "Texture.h"
#include "Timer.h"
#include "Projectile.h"

class Enemy : public Entity
{
public:
    Enemy(Point position, std::vector<std::shared_ptr<Projectile>> &projectiles);

    void Update(GameState& state) override;
    void Render(SDLRenderer& renderer) override;

    void SetMainTexture(std::shared_ptr<Texture> mainTexture);
    
protected:
    std::shared_ptr<Texture> m_mainTexture;
    float m_moveSpeed = 1.0f;
    float m_projectileSpeed = 10.0f;

    Timer m_fireTimer;

    std::vector<std::shared_ptr<Projectile>>& m_projectiles;
};