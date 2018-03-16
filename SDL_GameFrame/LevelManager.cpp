#include "LevelManager.h"

LevelManager* LevelManager::s_pSingleton = 0;

LevelManager::LevelManager()
{

}

LevelManager::~LevelManager()
{

}

bool LevelManager::init()
{
	addLevels();

	return true;
}

void LevelManager::addLevels()
{
	// add some level files to an array
	mLevelFiles["level1"] = "assets/map1.tmx";
	mLevelFiles["level2"] = "assets/map_temp.tmx";

}

void LevelManager::pushLevel(std::string level)
{

}