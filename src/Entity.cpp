#include "Entity.h"

Entity::Entity()
{
    m_position = { 0, 0 };
    m_bound = { 0, 0 };
    m_velocity = Vector2D(0, 0);
}

void Entity::Update(GameState& state)
{

}

void Entity::Render(SDLRenderer& renderer) 
{
    renderer.RenderRectangle(GetHitBox(), FG_COLOR.r, FG_COLOR.g, FG_COLOR.b, FG_COLOR.a);
}

Rectangle Entity::GetHitBox()
{
    return {{ m_position.x - m_bound.w / 2, m_position.y - m_bound.h / 2 }, m_bound};
}

bool Entity::IsActive()
{
    return m_isActive;
}