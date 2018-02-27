#ifndef __LevelScreenState__
#define __LevelScreenState__

#include <vector>
#include "GameObject.h"
#include "MenuState.h"

class LevelScreenState : public MenuState
{
public:
	//LevelScreenState();
	virtual ~LevelScreenState() {}

	virtual void update();
	virtual void render();

	virtual bool onEnter();
	virtual bool onExit();

	virtual std::string getStateID() const { return s_menuID; }

private:
	virtual	void setCallbacks(const std::vector<Callback>& callbacks);

	// callback functions for menu items
	static void s_menuToLevelOne();
	static void s_menuToLevelTwo();

	static const std::string s_menuID;

	std::vector<GameObject*> m_gameObjects;
};

#endif //__LevelScreenState__