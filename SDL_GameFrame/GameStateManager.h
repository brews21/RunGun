#ifndef __GameStateManager__
#define __GameStateManager__

#include <vector>
#include "GameState.h"

class GameStateManager
{
public:
    GameStateManager() {}
    ~GameStateManager() {}
    
    void update();
    void render();
    
    void pushState(GameState* pState);
    void changeState(GameState* pState);
    void popState();
    
    void clean();

	std::vector<GameState*>& getGameStates() { return m_gameStates; }
    
private:
    std::vector<GameState*> m_gameStates;
};

#endif // __GameStateManager__
