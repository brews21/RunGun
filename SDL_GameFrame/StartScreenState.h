#ifndef __StartScreenState__
#define __StartScreenState__

#include <iostream>
#include <vector>
#include "MenuState.h"

class GameObject;

class StartScreenState : public MenuState
{
public:
	virtual ~StartScreenState() {}

	virtual void update();
	virtual void render();

	virtual bool onEnter();
	virtual bool onExit();

	virtual std::string getStateID() const { return s_startScreenID; }

	virtual void setCallbacks(const std::vector<Callback>& callbacks);

private:
	static void s_toPlay();

	static const std::string s_startScreenID;

	std::vector<GameObject*> m_gameObjects;
};

#endif // __StartScreenState__