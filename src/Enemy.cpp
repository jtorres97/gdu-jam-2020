#include "Enemy.h"

Enemy::Enemy(Point position) : Entity()
{
    m_position = position;
    m_bound = { 48, 48 };
}

void Enemy::Update(GameState &state)
{
    // Chase player
    m_velocity = Vector2D(state.GetPlayerPosition() - m_position).Normalize().Scale(m_moveSpeed);
    m_position = m_position + m_velocity.GetPoint();
}

void Enemy::Render(SDLRenderer &renderer)
{
    renderer.RenderWholeTexture(m_mainTexture, GetHitBox());
}

void Enemy::SetMainTexture(std::shared_ptr<Texture> mainTexture)
{
    m_mainTexture = mainTexture;
}