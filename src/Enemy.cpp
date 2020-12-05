#include "Enemy.h"

Enemy::Enemy(Point position) : Entity()
{
    m_position = position;
    m_bound = { ENEMY_SIZE * TEXTURE_SCALE, ENEMY_SIZE * TEXTURE_SCALE };
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
}

void Enemy::Render(SDLRenderer &renderer)
{
    if (!m_isActive)
        return;

    renderer.RenderWholeTexture(m_mainTexture, GetHitBox());
}

void Enemy::SetMainTexture(std::shared_ptr<Texture> mainTexture)
{
    m_mainTexture = mainTexture;
}