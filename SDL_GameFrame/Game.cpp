#include "Game.h"
#include "Timer.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "Timer.h"

#include "MainMenuState.h"
#include "StartScreenState.h"

#include "GameObjectFactory.h"
#include "MenuButton.h"
#include "AnimatedGraphic.h"
#include "Player.h"
#include "ScrollingBackground.h"
#include "SoundManager.h"
#include "GameOverState.h"
#include "Snail.h"
#include <iostream>


using namespace std;

Game* Game::s_pSingleton = 0;

const int FPS = 60;
const int DELAY_TIME = 1000 / FPS;

Game::Game():
m_pWindow(0),
m_pRenderer(0),
m_bRunning(false),
m_pGameStateManager(0),
m_playerLives(3),
m_scrollSpeed(0.8),
m_bLevelComplete(false)
{
    // add some level files to an array
    m_levelFiles.push_back("assets/map1.tmx");
	m_levelFiles.push_back("assets/map_temp.tmx");
    
    // start at this level
    m_currentLevel = 0;
	m_nextLevel = 0;
}

Game::~Game()
{
    // we must clean up after ourselves to prevent memory leaks
    m_pRenderer= 0;
    m_pWindow = 0;
}


bool Game::init()
{
	// store the game width and height    
	m_gameWidth = 640;
	m_gameHeight = 480;
	
	// init the SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) >= 0)
	{
		// if it did init
		m_pWindow = SDL_CreateWindow(
			"SDL_GameFrame -- Orginal",			// Window Name
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,	// Window Position
			m_gameWidth,
			m_gameHeight,					// Window Size
			SDL_WINDOW_RESIZABLE);

		// creater the render
		if (m_pWindow != 0)
		{
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
		}
		else
		{
			return 1;
		}
	}

	TTF_Init();

	Timer::Singleton()->init();
	LevelManager::Singleton()->init();
    
    // add some sound effects - TODO move to better place
    SoundManager::Singleton()->load("assets/DST-Away.ogg", "music1", SOUND_MUSIC);
    SoundManager::Singleton()->load("assets/jump.wav", "jump", SOUND_SFX);
    SoundManager::Singleton()->load("assets/Laser_Shoot.wav", "shoot", SOUND_SFX);
    //SoundManager::Singleton()->load("assets/phaser.wav", "shoot", SOUND_SFX);
    
    //SoundManager::Singleton()->playMusic("music1", -1);
    
    
    InputManager::Singleton()->initialiseJoysticks();
    
    // register the types for the game
    GameObjectFactory::Singleton()->registerType("MenuButton", new MenuButtonCreator());
    GameObjectFactory::Singleton()->registerType("Player", new PlayerCreator());
    GameObjectFactory::Singleton()->registerType("AnimatedGraphic", new AnimatedGraphicCreator());
    GameObjectFactory::Singleton()->registerType("ScrollingBackground", new ScrollingBackgroundCreator());
    GameObjectFactory::Singleton()->registerType("Snail", new SnailCreator());

    // start the menu state
    m_pGameStateManager = new GameStateManager();
	m_pGameStateManager->changeState(new StartScreenState());

	//m_pLevelManager = new LevelManager();
  
	m_FPS = new TextManager("assets/DejaVuSans.ttf", 16, 0, 255, 0);

    m_bRunning = true; // everything inited successfully, start the main loop
    return true;
}

//void Game::setCurrentLevel(int currentLevel)
//{
//	m_pLevelManager->setCurrentLevel(currentLevel);
//    //m_pGameStateManager->changeState(new GameOverState());
//    m_bLevelComplete = false;
//}

void Game::render()
{
	SDL_RenderClear(m_pRenderer);

	m_pGameStateManager->render();

	m_FPS->renderText("FPS : " + to_string(Timer::Singleton()->getFPS()), 0, m_gameHeight - 16);

	SDL_RenderPresent(m_pRenderer);
}

void Game::update()
{
	Timer::Singleton()->Tick();
	m_pGameStateManager->update();

	handleEvents();
	render();
	framesPerSec();
}

void Game::framesPerSec()
{
	m_frameStart = SDL_GetTicks();
	m_frameTime = SDL_GetTicks() - m_frameStart;

	if (m_frameTime < DELAY_TIME)
	{
		SDL_Delay((int)(DELAY_TIME - m_frameTime));
	}
}

void Game::handleEvents()
{
    InputManager::Singleton()->update();

	if (InputManager::Singleton()->isKeyDown(SDL_SCANCODE_ESCAPE))
		quit();
}

void Game::clean()
{
    cout << "cleaning game\n";

	if (m_FPS)
	{
		delete m_FPS;
		m_FPS = nullptr;
	}
    
    InputManager::Singleton()->clean();
    
    m_pGameStateManager->clean();
    
    m_pGameStateManager = 0;
    delete m_pGameStateManager;
    
    TextureManager::Singleton()->clearTextureMap();
    
    SDL_DestroyWindow(m_pWindow);
    SDL_DestroyRenderer(m_pRenderer);
    SDL_Quit();
}


