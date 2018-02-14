#ifndef __RoofTurret__
#define __RoofTurret__

#include "Turret.h"

class RoofTurret : public Turret
{
public:
    
    virtual ~RoofTurret() {}
    
    virtual void draw()
    {
        TextureManager::Singleton()->drawFrame(m_textureID, (Uint32)m_position.getX(), (Uint32)m_position.getY(),
                                              m_width, m_height, m_currentRow, m_currentFrame, Game::Singleton()->getRenderer(), m_angle, m_alpha, SDL_FLIP_VERTICAL);
    }
    
    virtual void update()
    {
        if(!m_bDying)
        {
            // we want to scroll this object with the rest
            scroll(Game::Singleton()->getScrollSpeed());
            
            if(m_bulletCounter == m_bulletFiringSpeed)
            {
                BulletHandler::Singleton()->addEnemyBullet(m_position.getX(), m_position.getY() + 20, 16, 16, "bullet2", 1, Vector2D(-3, 3));
                BulletHandler::Singleton()->addEnemyBullet(m_position.getX() + 20, m_position.getY() + 20, 16, 16, "bullet2", 1, Vector2D(0, 3));
                BulletHandler::Singleton()->addEnemyBullet(m_position.getX() + 40, m_position.getY() + 20, 16, 16, "bullet2", 1, Vector2D(3, 3));
                m_bulletCounter = 0;
            }
            
            m_bulletCounter++;
        }
        else
        {
            scroll(Game::Singleton()->getScrollSpeed());
            doDyingAnimation();
        }
    }
};

class RoofTurretCreator : public BaseCreator
{
    GameObject* createGameObject() const
    {
        return new RoofTurret();
    }
};

#endif // __RoofTurret__