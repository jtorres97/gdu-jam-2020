#include "Enemy.h"

Enemy::Enemy(Point position) : Entity(), m_fireTimer(300)
{
    m_position = position;
    m_bound = { 8 * TEXTURE_SCALE, 8 * TEXTURE_SCALE };

    m_fireTimer.Reset();

    m_maxHP = 30;
    m_HP = m_maxHP;
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

    // Fire at the player continuously 
    // if (m_fireTimer.IsExpired())
    // {
    //     Vector2D projectileVel = vectorToPlayer.Scale(m_projectileSpeed);
    //     auto projectile = std::make_shared<Projectile>(m_position, projectileVel);
    //     m_projectiles.push_back(projectile);
    //     m_fireTimer.Reset();
    // }

    if (!IsAlive())
        m_isActive = false;
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