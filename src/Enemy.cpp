#include "Enemy.h"

Enemy::Enemy(Point position, std::vector<std::shared_ptr<Projectile>> &projectiles) : Entity(), m_fireTimer(300), m_projectiles(projectiles)
{
    m_position = position;
    m_bound = { 48, 48 };

    m_fireTimer.Reset();
}

void Enemy::Update(GameState &state)
{
    auto vectorToPlayer = Vector2D(state.GetPlayerPosition() - m_position).Normalize();

    // Chase player
    m_velocity = vectorToPlayer.Scale(m_moveSpeed);
    m_position = m_position + m_velocity.GetPoint();

    // Fire at the player continuously 
    if (m_fireTimer.IsExpired())
    {
        Vector2D projectileVel = vectorToPlayer.Scale(m_projectileSpeed);
        auto projectile = std::make_shared<Projectile>(m_position, projectileVel);
        m_projectiles.push_back(projectile);
        m_fireTimer.Reset();
    }
}

void Enemy::Render(SDLRenderer &renderer)
{
    renderer.RenderWholeTexture(m_mainTexture, GetHitBox());
}

void Enemy::SetMainTexture(std::shared_ptr<Texture> mainTexture)
{
    m_mainTexture = mainTexture;
}