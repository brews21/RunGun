#include "Player.h"
#include "Game.h"
#include "InputManager.h"
#include "TileLayer.h"
#include "SoundManager.h"
#include "Camera.h"
#include "TextureManager.h"
#include "BulletHandler.h"

#include "Timer.h"

using namespace std;

Player::Player() :  PlatformerObject(),
m_bInvulnerable(false),
m_invulnerableTime(200),
m_invulnerableCounter(0),
m_bPressedJump(false)
{
    m_jumpHeight = 100;
}

void Player::collision()
{
    if(!m_bDying && !m_bInvulnerable)
    {
        m_currentFrame = 0;
        m_currentRow = 4;
        m_numFrames = 9;
        m_width = 50;
        m_bDying = true;
        
        std::cout << m_currentFrame;
    }
}

void Player::load(std::unique_ptr<LoaderParams> const &pParams)
{
    // inherited load function
    PlatformerObject::load(std::move(pParams));
    
    // can set up the players inherited values here
    
    // set up bullets
    m_bulletFiringSpeed = 13;
    m_moveSpeed = 150; // pixels per second
    
    // we want to be able to fire instantly
    m_bulletCounter = m_bulletFiringSpeed;
    
    // time it takes for death explosion
    m_dyingTime = 100;
    
    Camera::Singleton()->setTarget(&m_position);
}

void Player::draw()
{
    if(m_bFlipped)
    {
        TextureManager::Singleton()->drawFrame(m_textureID, (Uint32)m_position.getX() -  Camera::Singleton()->getPosition().m_x, (Uint32)m_position.getY() -  Camera::Singleton()->getPosition().m_y,
                                              m_width, m_height, m_currentRow, m_currentFrame, Game::Singleton()->getRenderer(), m_angle, m_alpha, SDL_FLIP_HORIZONTAL);
    }
    else
    {
        TextureManager::Singleton()->drawFrame(m_textureID, (Uint32)m_position.getX() -  Camera::Singleton()->getPosition().m_x, (Uint32)m_position.getY() -  Camera::Singleton()->getPosition().m_y,
                                              m_width, m_height, m_currentRow, m_currentFrame, Game::Singleton()->getRenderer(), m_angle, m_alpha);
    }
}

void Player::handleAnimation()
{
    // if the player is invulnerable we can flash its alpha to let people know
    if(m_bInvulnerable)
    {
        // invulnerability is finished, set values back
        if(m_invulnerableCounter == m_invulnerableTime)
        {
            m_bInvulnerable = false;
            m_invulnerableCounter = 0;
            m_alpha = 255;
        }
        else // otherwise, flash the alpha on and off
        {
            if(m_alpha == 255)
            {
                m_alpha = 0;
            }
            else
            {
                m_alpha = 255;
            }
        }
        
        // increment our counter
        m_invulnerableCounter++;
    }
    
    // if the player is not dead then we can change the angle with the velocity to give the impression of a moving helicopter
    if(!m_bDead && !m_bDying)
    {
        if(m_velocity.m_y < 0)
        {
            m_currentFrame = 2;
            m_currentRow = 2;
            m_numFrames = 2;
        }
        else if(m_velocity.m_y > 0)
        {
            m_currentRow = 3;
            m_numFrames = 1;
        }
        else
        {
            if(m_velocity.getX() < 0)
            {
                m_currentRow = 1;
                m_numFrames = 4;
                m_bFlipped = true;
            }
            else if(m_velocity.getX() > 0)
            {
                m_currentRow = 1;
                m_numFrames = 4;
                m_bFlipped = false;
            }
            else
            {
                m_currentRow = 0;
                m_numFrames = 1;
            }
        }
        
        if(m_bRunning)
        {
            m_currentFrame = int(((SDL_GetTicks() / (100)) % m_numFrames));
        }
        else
        {
            m_currentFrame = int(((SDL_GetTicks() / (120)) % m_numFrames));
        }

    }
    else
    {
        m_currentFrame = m_dyingCounter / m_numFrames;//int(((SDL_GetTicks() / (200)) % m_numFrames));
    }
}

void Player::update()
{
    if(!m_bDying)
    {
		/*if (m_position.m_y + m_height >= 470)
		{
			collision();
		}*/

		float moveSpeed = m_moveSpeed * Timer::Singleton()->DeltaTime();

		handleInput();

		if (m_bMoveLeft)
		{
			if (m_bRunning)
			{
				m_velocity.m_x = -(moveSpeed * 2);
			}
			else
			{
				m_velocity.m_x = -moveSpeed;
			}
		}
		else if (m_bMoveRight)
		{
			if (m_bRunning)
			{
				m_velocity.m_x = (moveSpeed * 2);
			}
			else
			{
				m_velocity.m_x = moveSpeed;
			}
		}
		else
		{
			m_velocity.m_x = 0;
		}

		if (m_position.m_y < m_lastSafePos.m_y - m_jumpHeight)
		{
			m_bJumping = false;
		}

		if (!m_bJumping)
		{
			m_velocity.m_y = 5;
		}
		else
		{
			m_velocity.m_y = -5;
		}

		handleMovement(m_velocity);
	}
	else
	{
		m_velocity.m_x = 0;
		if (m_dyingCounter == m_dyingTime)
		{
			ressurect();
		}
		m_dyingCounter++;

		m_velocity.m_y = 5;
	}

    handleAnimation();

	//Vector2D pos = m_position - Camera::Singleton()->getPosition();
	//printf("Current Pos : x = %d -- y = %d \n", (Uint32)pos.getX(), (Uint32)pos.getY());
	//printf("Velocity  : x = %d -- y = %d \n", m_velocity.getX(), m_velocity.getY());
}

void Player::handleMovement(Vector2D velocity)
{
	// get the current position
	Vector2D newPos = m_position;

	// add velocity to the x position
	newPos.m_x = m_position.m_x + velocity.m_x;

	// check if the new x position would collide with a tile
	if (!checkCollideTile(newPos))
	{
		// no collision, add to the actual x position
		m_position.m_x = newPos.m_x;
	}
	else
	{
		// collision, stop x movement
		m_velocity.m_x = 0;
	}

	// get the current position after x movement
	newPos = m_position;

	// add velocity to y position
	newPos.m_y += velocity.m_y;

	// check if new y position would collide with a tile
	if (!checkCollideTile(newPos))
	{
		// no collision, add to the actual x position
		m_position.m_y = newPos.m_y;
	}
	else
	{
		// collision, stop y movement
		m_velocity.m_y = 0;

		//  we collided with the map which means we are safe on the ground,
		//  make this the last safe position
		m_lastSafePos = m_position;

		// move the safe pos slightly back or forward so when resurrected we are safely on the ground after a fall
		if (velocity.m_x > 0)
		{
			m_lastSafePos.m_x -= 32;
		}
		else if (velocity.m_x < 0)
		{
			m_lastSafePos.m_x += 32;

		}

		// allow the player to jump again
		m_bCanJump = true;

		// jumping is now false
		m_bJumping = false;
	}
}

void Player::ressurect()
{
    Game::Singleton()->setPlayerLives(Game::Singleton()->getPlayerLives() - 1);
    
	//m_position.setX(200);
	//m_position.setY(200);

    m_position = m_lastSafePos;
    m_bDying = false;
    
    m_currentFrame = 0;
    m_currentRow = 0;
    m_width = 40;
    
    m_dyingCounter = 0;
    m_bInvulnerable = true;
}

void Player::clean()
{
    PlatformerObject::clean();
}

void Player::handleInput()
{
	// left and right
	if (InputManager::Singleton()->isKeyDown(SDL_SCANCODE_D) && m_position.m_x < ((*m_pCollisionLayers->begin())->getMapWidth() * 32))
    {
		if (InputManager::Singleton()->isKeyDown(SDL_SCANCODE_LSHIFT))
		{
			m_bRunning = true;
		}
		else
		{
			m_bRunning = false;
		}

		m_bMoveRight = true;
		m_bMoveLeft = false;
    }
	else if (InputManager::Singleton()->isKeyDown(SDL_SCANCODE_A) && m_position.m_x > 32)
    {
		if (InputManager::Singleton()->isKeyDown(SDL_SCANCODE_LSHIFT))
		{
			m_bRunning = true;
		}
		else
		{
			m_bRunning = false;
		}

		m_bMoveRight = false;
		m_bMoveLeft = true;
    }
	else
	{
		m_bMoveRight = false;
		m_bMoveLeft = false;
	}

	//// up and down
	//if (InputManager::Singleton()->isKeyDown(SDL_SCANCODE_W))
	//{
	//	if (InputManager::Singleton()->isKeyDown(SDL_SCANCODE_LALT))
	//	{
	//
	//	}
	//}
	//else if (InputManager::Singleton()->isKeyDown(SDL_SCANCODE_S))
	//{
	//	if (InputManager::Singleton()->isKeyDown(SDL_SCANCODE_LALT))
	//	{
	//
	//	}
	//}
	//else
	//{
	//	
	//}

	// jumping
	//-----------------------------------------
    if(InputManager::Singleton()->isKeyDown(SDL_SCANCODE_SPACE) && m_bCanJump && !m_bPressedJump)
    {
        SoundManager::Singleton()->playSound("jump", 0);
        if(!m_bPressedJump)
        {
            m_bJumping = true;
            m_bCanJump = false;
            m_lastSafePos = m_position;
            m_bPressedJump = true;
        }
    }
    
    if(!InputManager::Singleton()->isKeyDown(SDL_SCANCODE_SPACE) && m_bCanJump)
    {
        m_bPressedJump = false;
    }
	//-----------------------------------------
	

	// shooting
	if (InputManager::Singleton()->getMouseButtonState(LEFT))
	{
		Vector2D pos = m_position - Camera::Singleton()->getPosition();

		Vector2D dir = *InputManager::Singleton()->getMousePosition() - pos;

		if (m_bulletCounter == m_bulletFiringSpeed)
		{
			SoundManager::Singleton()->playSound("shoot", 0);
			BulletHandler::Singleton()->addPlayerBullet(pos.getX(), pos.getY(), 11, 11, "bullet1", 1, dir.normalize() * 10);
			m_bulletCounter = 0;
		}

		m_bulletCounter++;
	}
	else
	{
		m_bulletCounter = m_bulletFiringSpeed;
	}


	// handle joysticks /
	if (InputManager::Singleton()->joysticksInitialised())
	{
		if (InputManager::Singleton()->getAxisX(0, 5) > 0)
		{
			if(m_bulletCounter == m_bulletFiringSpeed)
			{
				SoundManager::Singleton()->playSound("shoot", 0);
				BulletHandler::Singleton()->addPlayerBullet(m_position.getX() + 90, m_position.getY() + 12, 11, 11, "bullet1", 1, Vector2D(10,0));
				m_bulletCounter = 0;
			}

			m_bulletCounter++;
		}
		else
		{
			m_bulletCounter = m_bulletFiringSpeed;
		}

		if ((InputManager::Singleton()->getAxisX(0, 1) > 0 && m_position.m_x < ((*m_pCollisionLayers->begin())->getMapWidth() * 32)))
		{
			m_bMoveRight = true;
			m_bMoveLeft = false;
		}

		if ((InputManager::Singleton()->getAxisX(0, 1) < 0 && m_position.m_x < ((*m_pCollisionLayers->begin())->getMapWidth() * 32)))
		{
			m_bMoveRight = false;
			m_bMoveLeft = true;
		}

		if ((((InputManager::Singleton()->getButtonState(0, 10) && (m_position.getY() + m_height) < Game::Singleton()->getGameHeight() - 10) || (InputManager::Singleton()->getButtonState(0, 10) && m_position.getY() > 0))) && m_bCanJump && !m_bPressedJump)
		{
			m_bJumping = true;
			m_bCanJump = false;
			m_lastSafePos = m_position;
			m_bPressedJump = true;
		}

		// testing the button ranges
		if (InputManager::Singleton()->getButtonState(0, 1))
			cout << "1" << endl;
		if (InputManager::Singleton()->getButtonState(0, 2))
			cout << "2" << endl;
		if (InputManager::Singleton()->getButtonState(0, 3))
			cout << "3" << endl;
		if (InputManager::Singleton()->getButtonState(0, 4))
			cout << "4" << endl;


	}
	//
	
}