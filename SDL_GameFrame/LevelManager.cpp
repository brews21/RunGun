#include "LevelManager.h"

LevelManager::LevelManager()
{
	addLevels();

}


LevelManager::~LevelManager()
{
}


void LevelManager::addLevels()
{
	// add some level files to an array
	m_levelFiles.push_back("assets/map1.tmx");
	m_levelFiles.push_back("assets/map_temp.tmx");

	// start at this level
	m_currentLevel = 0;
	m_nextLevel = 0;
}

void LevelManager::pushLevel(std::string level)
{

}