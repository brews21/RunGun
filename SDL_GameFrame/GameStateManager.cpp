#include "GameStateManager.h"
#include <iostream>

void GameStateManager::clean()
{
    if(!m_gameStates.empty())
    {
        m_gameStates.back()->onExit();

        delete m_gameStates.back();
        
        m_gameStates.clear();
    }
}

void GameStateManager::update()
{
    if(!m_gameStates.empty())
    {
        m_gameStates.back()->update();
	}
}

void GameStateManager::render()
{
    if(!m_gameStates.empty())
    {
        m_gameStates.back()->render();
    }
}

void GameStateManager::pushState(GameState *pState)
{
    m_gameStates.push_back(pState);
    m_gameStates.back()->onEnter();
}

void GameStateManager::popState()
{
    if(!m_gameStates.empty())
    {
        m_gameStates.back()->onExit();
        m_gameStates.pop_back();
    }
    
    m_gameStates.back()->resume();
}

void GameStateManager::changeState(GameState *pState)
{
    if(!m_gameStates.empty())
    {
        if(m_gameStates.back()->getStateID() == pState->getStateID())
        {
            return; // do nothing
        }

		// clean up previous state
		//m_gameStates.back()->onExit();
		//m_gameStates.pop_back();
    }

	// init new state
	pushState(pState);
}