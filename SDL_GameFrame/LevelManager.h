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

	void setCurrentLevel(string currentLevel) { mcurrentLevel = currentLevel; }
	const string getCurrentLevel_string() { return mLevelFiles[mcurrentLevel]; }

private:
	static LevelManager* s_pSingleton;

	LevelManager();
	~LevelManager();

	LevelManager(const LevelManager&);

	void addLevels();
	
	map<string, string> mLevelFiles;
	string mcurrentLevel;
};

#endif // __LevelManager__