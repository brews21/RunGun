#include "LevelScreenState.h"

#include <iostream>
#include "TextureManager.h"
#include "Game.h"
#include "MenuButton.h"
#include "PlayState.h"
#include "InputManager.h"
#include "StateParser.h"

#include "MainMenuState.h"

const std::string LevelScreenState::s_menuID = "LEVELSCREEN";

void LevelScreenState::s_menuToLevelOne()
{
	Game::Singleton()->setCurrentLevel(0);
	Game::Singleton()->getStateMachine()->changeState(new PlayState);
}

void LevelScreenState::s_menuToLevelTwo()
{
	Game::Singleton()->setCurrentLevel(1);
	Game::Singleton()->getStateMachine()->changeState(new PlayState);
}

void LevelScreenState::update()
{
	if (m_loadingComplete && !m_gameObjects.empty() && !m_exiting)
	{
		for (int i = 0; i < m_gameObjects.size(); i++)
		{
			m_gameObjects[i]->update();
		}

		//        if(InputManager::Singleton()->getButtonState(0, 8))
		//        {
		//            s_menuToPlay();
		//        }
	}

}

void LevelScreenState::render()
{
	if (m_loadingComplete && !m_gameObjects.empty())
	{
		for (int i = 0; i < m_gameObjects.size(); i++)
		{
			m_gameObjects[i]->draw();
		}
	}
}

bool LevelScreenState::onEnter()
{
	InputManager::Singleton()->reset();

	// parse the state
	StateParser stateParser;
	stateParser.parseState("assets/fmp.xml", s_menuID, &m_gameObjects, &m_textureIDList);

	m_callbacks.push_back(0);
	m_callbacks.push_back(s_menuToLevelOne);
	m_callbacks.push_back(s_menuToLevelTwo);

	// set the callbacks for menu items
	setCallbacks(m_callbacks);

	m_loadingComplete = true;
	std::cout << "entering LevelScreenState\n";
	return true;
}

bool LevelScreenState::onExit()
{
	m_exiting = true;

	// clean the game objects
	if (m_loadingComplete && !m_gameObjects.empty())
	{
		m_gameObjects.back()->clean();
		m_gameObjects.pop_back();
	}

	m_gameObjects.clear();


	/* clear the texture manager
	for(int i = 0; i < m_textureIDList.size(); i++)
	{
	TextureManager::Singleton()->clearFromTextureMap(m_textureIDList[i]);
	}
	*/

	// reset the input handler
	InputManager::Singleton()->reset();

	std::cout << "exiting MenuState\n";
	return true;
}

void LevelScreenState::setCallbacks(const std::vector<Callback>& callbacks)
{
	// go through the game objects
	if (!m_gameObjects.empty())
	{
		for (int i = 0; i < m_gameObjects.size(); i++)
		{
			// if they are of type MenuButton then assign a callback based on the id passed in from the file
			if (dynamic_cast<MenuButton*>(m_gameObjects[i]))
			{
				MenuButton* pButton = dynamic_cast<MenuButton*>(m_gameObjects[i]);
				pButton->setCallback(callbacks[pButton->getCallbackID()]);
			}
		}
	}
}