#pragma once

#include "Entity.h"
#include "Texture.h"

class Projectile : public Entity
{
public:
    Projectile(Point position, Vector2D velocity);
    void Update(GameState &state) override;
    void Render(SDLRenderer &renderer) override;

    void SetMainTexture(std::shared_ptr<Texture> mainTexture);
    
protected:
    std::shared_ptr<Texture> m_mainTexture;
    int m_damage = 10;
};