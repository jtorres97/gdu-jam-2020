#pragma once

#include "Entity.h"
#include "Texture.h"
#include "Timer.h"

const int ENEMY_SIZE = 8;

enum class EnemyDirection
{
    UP, 
    DOWN,
    LEFT,
    RIGHT
};

class Enemy : public Entity
{
public:
    EnemyDirection direction;
    Enemy(Point position);

    void Update(GameState& state) override;
    void Render(SDLRenderer& renderer) override;

    void SetMainTexture(std::shared_ptr<Texture> mainTexture);
    
protected:
    std::shared_ptr<Texture> m_mainTexture;
    float m_moveSpeed = 8.0f;
};