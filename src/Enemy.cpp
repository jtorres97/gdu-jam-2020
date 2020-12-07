#include "Enemy.h"

Enemy::Enemy(Point position, float speed, std::shared_ptr<Animation> animation) : Entity()
{
    m_position = position;
    m_bound = { ENEMY_SIZE * TEXTURE_SCALE, ENEMY_SIZE * TEXTURE_SCALE };

    m_moveSpeed = speed;

    m_mainAnimation = animation;
}

void Enemy::Update(GameState &state)
{
    if (!m_isActive)
        return;

    Point center = {WORLDSIZE_W / 2, WORLDSIZE_H / 2};
    auto vectorToPlayer = Vector2D(center - m_position).Normalize();

    // Chase player
    m_velocity = vectorToPlayer.Scale(m_moveSpeed);
    m_position = m_position + m_velocity.GetPoint();

    m_mainAnimation->Update();
}

void Enemy::Render(SDLRenderer &renderer)
{
    if (!m_isActive)
        return;

    m_mainAnimation->Render(renderer, m_position.x - m_bound.w / 2, m_position.y - m_bound.h / 2);
}