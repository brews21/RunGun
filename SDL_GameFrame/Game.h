#ifndef __Game__
#define __Game__

#include "SDL.h"
#include "GameStateManager.h"
#include "TextManager.h"
#include <vector>
#include "LevelManager.h"

// game.h from master
// from MikeyBLaptop
class Game
{
public:
    
    static Game* Singleton()
    {
        if(s_pSingleton == 0)
        {
            s_pSingleton = new Game();
            return s_pSingleton;
        }
        
        return s_pSingleton;
    }

    
    bool init();
    
    void render();
    void update();
    void handleEvents();
    void clean();
    
    SDL_Renderer* getRenderer() const { return m_pRenderer; }
    SDL_Window* getWindow() const { return m_pWindow; }
    
	GameStateManager* getStateMachine() { return m_pGameStateManager; }
    
    void setPlayerLives(int lives) { m_playerLives = lives; }
    int getPlayerLives() { return m_playerLives; }
    
    bool running() { return m_bRunning; }
    
    void quit() { m_bRunning = false; }
    
    int getGameWidth() const { return m_gameWidth; }
    int getGameHeight() const { return m_gameHeight; }
	float getScrollSpeed() { return m_scrollSpeed; }

	void framesPerSec();

private:
    
    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;
    
    GameStateManager* m_pGameStateManager;
	
    bool m_bRunning;
    
    static Game* s_pSingleton;
    
    int m_gameWidth;
    int m_gameHeight;
    int m_playerLives;

	float m_scrollSpeed;

	// Timer
	Uint32 m_frameStart, m_frameTime;
    
    Game();
    ~Game();
    
    Game(const Game&);
	Game& operator=(const Game&);

	TextManager* m_FPS;
};

typedef Game Game;

#endif // __Game__