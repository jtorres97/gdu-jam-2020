#include "Projectile.h"

Projectile::Projectile(Point position, Vector2D velocity)
{
    m_position = position;
    m_velocity = velocity;

    m_bound = {12, 12};
}

void Projectile::Update(GameState &state)
{
    m_position = m_position + m_velocity.GetPoint();
}

void Projectile::Render(SDLRenderer &renderer)
{
    // TODO: add projectile texture
    // renderer.RenderWholeTexture(m_mainTexture, GetHitBox());
    renderer.RenderRectangle(GetHitBox(), FG_COLOR.r, FG_COLOR.g, FG_COLOR.b, FG_COLOR.a);
}

void Projectile::SetMainTexture(std::shared_ptr<Texture> mainTexture)
{
    m_mainTexture = mainTexture;
}
