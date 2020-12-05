#pragma once

#include "Geometry.h"
#include "Renderer.h"
#include "GameState.h"
#include "Vector.h"

class Entity
{
public:
    virtual void Update(GameState& state);
    virtual void Render(SDLRenderer& renderer);

    Rectangle GetHitBox();
    bool IsActive();
    
protected:
    Entity();

    bool m_isActive = true;
    Point m_position;
    Size m_bound;
    Vector2D m_velocity;
};