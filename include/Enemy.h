#pragma once

#include "Entity.h"
#include "Animation.h"
#include "Timer.h"

const int ENEMY_SIZE = 28;

enum class EnemyDirection
{
    UNKNOWN,
    UP, 
    DOWN,
    LEFT,
    RIGHT
};

class Enemy : public Entity
{
public:
    EnemyDirection direction;
    Enemy(Point position, float speed, std::shared_ptr<Animation> animation);

    void Update(GameState& state) override;
    void Render(SDLRenderer& renderer) override;

protected:
    std::shared_ptr<Animation> m_mainAnimation;
    float m_moveSpeed = 0.0f;
};