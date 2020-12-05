#pragma once

#include <vector>

#include "Entity.h"
#include "Timer.h"
#include "Enemy.h"

class Spawner : public Entity
{
public:
    Spawner(Point position,
            std::vector<std::shared_ptr<Enemy>> &enemies,
            std::vector<std::shared_ptr<Projectile>> &projectiles);

    void Update(GameState &state) override;
    void Render(SDLRenderer &renderer) override;

    void SetMainTexture(std::shared_ptr<Texture> mainTexture);
    void SetEnemyTexture(std::shared_ptr<Texture> enemyTexture);

protected:
    std::shared_ptr<Texture> m_mainTexture;
    std::shared_ptr<Texture> m_enemyTexture;
    std::vector<std::shared_ptr<Enemy>> &m_enemies;
    std::vector<std::shared_ptr<Projectile>> &Projectiles;

    Timer m_spawnTimer;
};