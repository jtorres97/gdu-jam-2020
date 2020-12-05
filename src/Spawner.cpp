#include "Spawner.h"

Spawner::Spawner(Point position,
                 std::vector<std::shared_ptr<Enemy>> &enemies,
                 std::vector<std::shared_ptr<Projectile>> &projectiles) : m_spawnTimer(4000), m_enemies(enemies), m_projectiles(projectiles), Entity()
{
    m_position = position;
    m_bound = {128, 128};

    m_spawnTimer.Reset();

    m_maxHP = 200;
    m_HP = m_maxHP;
}

void Spawner::Update(GameState &state)
{
    if (!m_isActive)
        return;

    // Spawn additional enemies
    if (m_spawnTimer.IsExpired())
    {
        Point enemyPosition = m_position + Point{0, -100};
        auto ne = std::make_shared<Enemy>(enemyPosition, m_projectiles);
        ne->SetMainTexture(m_enemyTexture);
        m_enemies.push_back(ne);
        m_spawnTimer.Reset();
    }

    if (!IsAlive())
        m_isActive = false;
}

void Spawner::Render(SDLRenderer &renderer)
{
    if (!m_isActive)
        return;
        
    renderer.RenderWholeTexture(m_mainTexture, GetHitBox());
}

void Spawner::SetMainTexture(std::shared_ptr<Texture> mainTexture)
{
    m_mainTexture = mainTexture;
}

void Spawner::SetEnemyTexture(std::shared_ptr<Texture> enemyTexture)
{
    m_enemyTexture = enemyTexture;
}