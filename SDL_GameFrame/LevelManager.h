#ifndef __LevelManager__
#define __LevelManager__

#include <string>


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

	void pushLevel(std::string level);

	

private:
	static LevelManager* s_pSingleton;

	LevelManager();
	~LevelManager();
};

#endif // __LevelManager__