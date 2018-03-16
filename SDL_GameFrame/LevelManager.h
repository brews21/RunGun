#ifndef __LevelManager__
#define __LevelManager__

#include <string>
#include <vector>
#include <map>

using namespace std;

class LevelManager
{
public:

	static LevelManager* Singleton()
	{
		if (s_pSingleton == 0)
		{
			s_pSingleton = new LevelManager();
		}

		return s_pSingleton;
	}

	bool init();

	void pushLevel(string level);

	void setCurrentLevel(int currentLevel) { m_currentLevel = currentLevel;  }
	const int getCurrentLevel() { return m_currentLevel; }

	void setCurrentLevel(string currentLevel) { mcurrentLevel = currentLevel; }
	const string getCurrentLevel_string() { return mLevelFiles[mcurrentLevel]; }

	void setNextLevel(int nextLevel) { m_nextLevel = nextLevel; }
	const int getNextLevel() { return m_nextLevel; }

	void setLevelComplete(bool levelComplete) { m_bLevelComplete = levelComplete; }
	const bool getLevelComplete() { return m_bLevelComplete; }

	vector<string> getLevelFiles() { return m_levelFiles; }

private:
	static LevelManager* s_pSingleton;

	LevelManager();
	~LevelManager();

	LevelManager(const LevelManager&);

	void addLevels();
	
	int m_currentLevel;
	int m_nextLevel;
	bool m_bLevelComplete;

	vector<string> m_levelFiles;

	map<string, string> mLevelFiles;
	string mcurrentLevel;
};

#endif // __LevelManager__