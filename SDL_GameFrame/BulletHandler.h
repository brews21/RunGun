#ifndef __BulletHandler__
#define __BulletHandler__

#include <iostream>
#include <vector>
#include "Bullet.h"

class BulletHandler
{
public:
    
    static BulletHandler* Singleton()
    {
        if(s_pSingleton == 0)
        {
            s_pSingleton = new BulletHandler();
            return s_pSingleton;
        }
        return s_pSingleton;
    }
    
    void addPlayerBullet(int x, int y, int width, int height, std::string textureID, int numFrames, Vector2D heading);
    void addEnemyBullet(int x, int y, int width, int height, std::string textureID, int numFrames, Vector2D heading);
    
    void updateBullets();
    void drawBullets();
    
    void clearBullets();
    
    const std::vector<PlayerBullet*> getPlayerBullets() { return m_playerBullets; }
    const std::vector<EnemyBullet*> getEnemyBullets() { return m_enemyBullets; }
    
private:
    
    BulletHandler();
    ~BulletHandler();
    
    BulletHandler(const BulletHandler&);
    BulletHandler& operator=(const BulletHandler&);
    
    static BulletHandler* s_pSingleton;
    
    // in play bullets
    std::vector<PlayerBullet*> m_playerBullets;
    std::vector<EnemyBullet*> m_enemyBullets;
};

typedef BulletHandler BulletHandler;

#endif //__BulletHandler__
