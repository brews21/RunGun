#include "LevelScreenState.h"

#include <iostream>
#include "TextureManager.h"
#include "Game.h"
#include "MenuButton.h"
#include "PlayState.h"
#include "InputManager.h"
#include "StateParser.h"

const std::string LevelScreenState::s_menuID = "LEVELSCREEN";

void LevelScreenState::s_menuToLevelOne()
{

}

void LevelScreenState::s_menuToLevelTwo()
{

}

void LevelScreenState::update()
{

}

void LevelScreenState::render()
{

}

bool LevelScreenState::onEnter()
{

}

bool LevelScreenState::onExit()
{

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