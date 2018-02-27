#include <iostream>
#include "PlayState.h"
#include "GameOverState.h"
#include "PauseState.h"
#include "Game.h"
#include "InputManager.h"
#include "LevelParser.h"
#include "Level.h"
#include "TextureManager.h"
#include "BulletHandler.h"

const std::string PlayState::s_playID = "PLAY";

void PlayState::update()
{
    if(m_loadingComplete && !m_exiting)
    {
        if(InputManager::Singleton()->isKeyDown(SDL_SCANCODE_BACKSPACE))
        {
            Game::Singleton()->getStateMachine()->pushState(new PauseState());
        }
        
//        if(InputManager::Singleton()->getButtonState(0, 8))
//        {
//            Game::Singleton()->getStateMachine()->pushState(new PauseState());
//        }
        
        if(Game::Singleton()->getPlayerLives() == 0)
        {
            Game::Singleton()->getStateMachine()->changeState(new GameOverState());
        }
        
		if (m_pLevel != 0)
        {
			m_pLevel->update();
        }

		// update bullets
		BulletHandler::Singleton()->updateBullets();
    }
}

void PlayState::render()
{
    if(m_loadingComplete)
    {
        if(m_pLevel != 0)
        {
			m_pLevel->render();
        }
        
        for(int i = 0; i < Game::Singleton()->getPlayerLives(); i++)
        {
            TextureManager::Singleton()->drawFrame("lives", i * 30, 0, 32, 30, 0, 0, Game::Singleton()->getRenderer(), 0.0, 255);
        }

		// draw cross hair for mouse
		TextureManager::Singleton()->drawFrame("crosshair", InputManager::Singleton()->getMousePosition()->getX() - 16, InputManager::Singleton()->getMousePosition()->getY() - 16, 32, 30, 0, 0, Game::Singleton()->getRenderer(), 0, 255);
		
		// draw bullets
		BulletHandler::Singleton()->drawBullets();
	}
}

bool PlayState::onEnter()
{
    Game::Singleton()->setPlayerLives(3);
    
    LevelParser levelParser;
	m_pLevel = levelParser.parseLevel(Game::Singleton()->getLevelFiles()[Game::Singleton()->getCurrentLevel()].c_str());
    
	TextureManager::Singleton()->load("assets/bullet1.png", "bullet1", Game::Singleton()->getRenderer());
	TextureManager::Singleton()->load("assets/bullet2.png", "bullet2", Game::Singleton()->getRenderer());
	TextureManager::Singleton()->load("assets/bullet3.png", "bullet3", Game::Singleton()->getRenderer());
	TextureManager::Singleton()->load("assets/lives.png", "lives", Game::Singleton()->getRenderer());
	TextureManager::Singleton()->load("assets/crosshair.png", "crosshair", Game::Singleton()->getRenderer());

	if (m_pLevel != 0)
    {
        m_loadingComplete = true;
    }
    
    std::cout << "entering PlayState\n";
    return true;
}

bool PlayState::onExit()
{
    m_exiting = true;
    
    InputManager::Singleton()->reset();
    
    std::cout << "exiting PlayState\n";
    return true;
}
