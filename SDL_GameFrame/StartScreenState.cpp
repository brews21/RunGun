#include "StartScreenState.h"
#include "MainMenuState.h"
#include "PlayState.h"
#include "TextureManager.h"
#include "AnimatedGraphic.h"
#include "Game.h"
#include "MenuButton.h"
#include "InputManager.h"
#include "StateParser.h"

const std::string StartScreenState::s_startScreenID = "STARTSCREEN";

void StartScreenState::s_toPlay()
{
	Game::Singleton()->getStateMachine()->changeState(new MainMenuState());
}

void StartScreenState::update()
{
	if (m_loadingComplete && !m_gameObjects.empty())
	{
		for (int i = 0; i < m_gameObjects.size(); i++)
		{
			m_gameObjects[i]->update();
		}
	}

	if (InputManager::Singleton()->isKeyDown(SDL_SCANCODE_SPACE))
		Game::Singleton()->getStateMachine()->changeState(new MainMenuState());
}

void StartScreenState::render()
{
	if (m_loadingComplete && !m_gameObjects.empty())
	{
		for (int i = 0; i < m_gameObjects.size(); i++)
		{
			m_gameObjects[i]->draw();
		}
	}
}

bool StartScreenState::onEnter()
{
	// parse the state
	StateParser stateParser;
	stateParser.parseState("assets/fmp.xml", s_startScreenID, &m_gameObjects, &m_textureIDList);

	m_callbacks.push_back(0);
	m_callbacks.push_back(s_toPlay);

	// set the callbacks for menu items
	setCallbacks(m_callbacks);

	m_loadingComplete = true;

	std::cout << "entering StartScreenState\n";
	return true;
}

bool StartScreenState::onExit()
{
	if (m_loadingComplete && !m_gameObjects.empty())
	{
		for (int i = 0; i < m_gameObjects.size(); i++)
		{
			m_gameObjects[i]->clean();
			delete m_gameObjects[i];
		}

		m_gameObjects.clear();
	}

	std::cout << m_gameObjects.size();

	// clear the texture manager
	for (int i = 0; i < m_textureIDList.size(); i++)
	{
		TextureManager::Singleton()->clearFromTextureMap(m_textureIDList[i]);
	}

	InputManager::Singleton()->reset();

	std::cout << "exiting StartScreenState\n";
	return true;
}

void StartScreenState::setCallbacks(const std::vector<Callback>& callbacks)
{
	// go through the game objects
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

