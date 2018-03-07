#ifndef __LevelManager__
#define __LevelManager__

#include <string>
#include <vector>

class LevelManager
{
public:

	/*static LevelManager* Singleton()
	{
		if (s_pSingleton == 0)
		{
			s_pSingleton = new LevelManager();
		}

		return s_pSingleton;
	}*/


	LevelManager();
	~LevelManager();


	void pushLevel(std::string level);

	void setCurrentLevel(int currentLevel) { m_currentLevel = currentLevel;  }
	const int getCurrentLevel() { return m_currentLevel; }

	void setNextLevel(int nextLevel) { m_nextLevel = nextLevel; }
	const int getNextLevel() { return m_nextLevel; }

	void setLevelComplete(bool levelComplete) { m_bLevelComplete = levelComplete; }
	const bool getLevelComplete() { return m_bLevelComplete; }

	std::vector<std::string> getLevelFiles() { return m_levelFiles; }

private:
	//static LevelManager* s_pSingleton;

	void addLevels();
	
	int m_currentLevel;
	int m_nextLevel;
	bool m_bLevelComplete;

	std::vector<std::string> m_levelFiles;
};

#endif // __LevelManager__