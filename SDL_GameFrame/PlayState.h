#ifndef __PlayState__
#define __PlayState__

#include "GameState.h"
#include "CollisionManager.h"
#include "Level.h"
#include <vector>

class GameObject;
class SDLGameObject;
class Level;

class PlayState : public GameState
{
public:
    
	virtual ~PlayState() { delete m_pLevel; }
    
    virtual void update();
    virtual void render();
    
    virtual bool onEnter();
    virtual bool onExit();
    
    virtual std::string getStateID() const { return s_playID; }
    
private:
    
    static const std::string s_playID;
    
    //CollisionManager m_collisionManager;
    
    std::vector<GameObject*> m_pGameObjects;
    
    Level* m_pLevel;
};

#endif // __PlayState__