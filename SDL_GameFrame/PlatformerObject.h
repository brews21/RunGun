// This needs to be renamed to a better base class for the objects in the game
// maye change GameObject to Object and this to GameObject

#ifndef __SDLGameObject__
#define __SDLGameObject__

#include <SDL.h>
#include "GameObject.h"

class PlatformerObject : public GameObject
{
public:
    
    virtual ~PlatformerObject() {}

    virtual void load(std::unique_ptr<LoaderParams> const &pParams);
    
    virtual void draw();
    virtual void update();
    
    virtual void clean() {}
    virtual void collision() {}
    
    virtual std::string type() { return "PlatformerObject"; }
    
protected:
    
    PlatformerObject();
    
    bool checkCollideTile(Vector2D newPos);
       
    void doDyingAnimation();
    
    int m_bulletFiringSpeed;
    int m_bulletCounter;
    int m_moveSpeed;
    
    // how long the death animation takes, along with a counter
    int m_dyingTime;
    int m_dyingCounter;
    
    // has the explosion sound played?
    bool m_bPlayedDeathSound;
    
    bool m_bFlipped;
    
    bool m_bMoveLeft;
    bool m_bMoveRight;
    bool m_bRunning;
    
    bool m_bFalling;
    bool m_bJumping;
    bool m_bCanJump;
    
    Vector2D m_lastSafePos;
    
    int m_jumpHeight;
};

#endif // __SDLGameObject__
