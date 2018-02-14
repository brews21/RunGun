// look at changing to a boss class used for all bosses

#ifndef __Level1Boss__
#define __Level1Boss__

#include "Enemy.h"
#include "GameObjectFactory.h"
#include "SoundManager.h"
#include "Game.h"
#include "BulletHandler.h"

class Level1Boss : public Enemy
{
public:
    
    virtual ~Level1Boss() {}
    
    Level1Boss() : Enemy()
    {
        m_health = 100;
        m_dyingTime = 100;
        m_bulletFiringSpeed = 100;
        
        m_moveSpeed = 2;
        
        m_entered = false;
    }
    
    virtual void load(LoaderParams &pParams)
    {
        PlatformerObject::load(std::move(pParams));
        
        m_velocity.setY(-m_moveSpeed);
    }
    
    virtual void collision()
    {
        if(m_entered)
        {
            m_health -= 1;
            
            if(m_health == 0)
            {
                if(!m_bPlayedDeathSound)
                {
                    m_position.setX(m_position.getX() + 30);
                    m_position.setY(m_position.getY() + 70);
                    SoundManager::Singleton()->playSound("explode", 0);
                    
                    m_textureID = "bossexplosion";
                    m_currentFrame = 0;
                    m_numFrames = 9;
                    m_width = 180;
                    m_height = 180;
                    m_bDying = true;
                }
                
            }
        }
    }
    
    virtual void update()
    {
        if(!m_entered)
        {
            scroll(Game::Singleton()->getScrollSpeed());
            
            if(m_position.getX() < (Game::Singleton()->getGameWidth() - (m_width + 20)))
            {
                m_entered = true;
            }
        }
        else
        {
            if(!m_bDying)
            {
                if(m_position.getY() + m_height >= Game::Singleton()->getGameHeight())
                {
                    m_velocity.setY(-m_moveSpeed);
                }
                else if(m_position.getY() <= 0)
                {
                    m_velocity.setY(m_moveSpeed);
                }

                if(m_bulletCounter == m_bulletFiringSpeed)
                {
                    BulletHandler::Singleton()->addEnemyBullet(m_position.getX(), m_position.getY() + 15, 16, 16, "bullet2", 1, Vector2D(-10, 0));
                    BulletHandler::Singleton()->addEnemyBullet(m_position.getX(), m_position.getY() + 25, 16, 16, "bullet2", 1, Vector2D(-10, 0));
                    
                    BulletHandler::Singleton()->addEnemyBullet(m_position.getX(), m_position.getY() + 200, 16, 16, "bullet2", 1, Vector2D(-10, 0));
                    BulletHandler::Singleton()->addEnemyBullet(m_position.getX(), m_position.getY() + 215, 16, 16, "bullet2", 1, Vector2D(-10, 0));
                    
                    m_bulletCounter = 0;
                }
                
                m_bulletCounter++;
                
                PlatformerObject::update();
            }
            else
            {
                scroll(Game::Singleton()->getScrollSpeed());
                m_currentFrame = int(((SDL_GetTicks() / (1000 / 3)) % m_numFrames));
                
                if(m_dyingCounter == m_dyingTime)
                {
                    m_bDead = true;
                    Game::Singleton()->setLevelComplete(true);
                }
                m_dyingCounter++;

            }

        }
    }
    
private:
    
    bool m_entered;
};

class Level1BossCreator : public BaseCreator
{
    GameObject* createGameObject() const
    {
        return new Level1Boss();
    }
};


#endif // __Level1Boss