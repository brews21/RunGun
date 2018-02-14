#include "Game.h"

int main(int argc, char* argv[])
{
	if (Game::Singleton()->init())
	{
		while (Game::Singleton()->running())
			Game::Singleton()->update();
	}

	// when the game ends 
	Game::Singleton()->clean();

	return 0;
}