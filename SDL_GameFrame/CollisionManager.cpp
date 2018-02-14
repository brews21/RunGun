#include "CollisionManager.h"
#include "Collision.h"
#include "Player.h"
#include "Enemy.h"
#include "BulletHandler.h"
#include "TileLayer.h"
#include "Camera.h"

void CollisionManager::checkPlayerEnemyBulletCollision(Player* pPlayer)
{
	SDL_Rect* pRect1 = new SDL_Rect();
	pRect1->x = (int)pPlayer->getPosition().getX();
	pRect1->y = (int)pPlayer->getPosition().getY();
	pRect1->w = (int)pPlayer->getWidth();
	pRect1->h = (int)pPlayer->getHeight();

	for (unsigned int i = 0; i < BulletHandler::Singleton()->getEnemyBullets().size(); i++)
	{
		EnemyBullet* pEnemyBullet = BulletHandler::Singleton()->getEnemyBullets()[i];

		SDL_Rect* pRect2 = new SDL_Rect();
		pRect2->x = (int)pEnemyBullet->getPosition().getX();
		pRect2->y = (int)pEnemyBullet->getPosition().getY();

		pRect2->w = (int)pEnemyBullet->getWidth();
		pRect2->h = (int)pEnemyBullet->getHeight();

		if (AABBCollision(pRect1, pRect2))
		{
			if (!pPlayer->dying() && !pEnemyBullet->dying())
			{
				pEnemyBullet->collision();
				pPlayer->collision();
			}
		}

		delete pRect2;
	}

	delete pRect1;
}

void CollisionManager::checkEnemyPlayerBulletCollision(const std::vector<GameObject *> &objects)
{
	for (int i = 0; i < objects.size(); i++)
	{
		GameObject* pObject = objects[i];

		for (unsigned int j = 0; j < BulletHandler::Singleton()->getPlayerBullets().size(); j++)
		{
			if (pObject->type() != std::string("Enemy") || !pObject->updating())
			{
				continue;
			}

			SDL_Rect* pRect1 = new SDL_Rect();
			pRect1->x = (Uint32)pObject->getPosition().getX() - Camera::Singleton()->getPosition().m_x;
			pRect1->y = (Uint32)pObject->getPosition().getY() - Camera::Singleton()->getPosition().m_y;
			pRect1->w = pObject->getWidth();
			pRect1->h = pObject->getHeight();

			PlayerBullet* pPlayerBullet = BulletHandler::Singleton()->getPlayerBullets()[j];

			SDL_Rect* pRect2 = new SDL_Rect();
			pRect2->x = pPlayerBullet->getPosition().getX();
			pRect2->y = pPlayerBullet->getPosition().getY();
			pRect2->w = pPlayerBullet->getWidth();
			pRect2->h = pPlayerBullet->getHeight();

			if (AABBCollision(pRect1, pRect2))
			{
				if (!pObject->dying() && !pPlayerBullet->dying())
				{
					pPlayerBullet->collision();
					pObject->collision();
				}

			}

			delete pRect1;
			delete pRect2;
		}
	}
}

void CollisionManager::checkPlayerEnemyCollision(Player* pPlayer, const std::vector<GameObject*> &objects)
{
	SDL_Rect* pRect1 = new SDL_Rect();
	pRect1->x = pPlayer->getPosition().getX();
	pRect1->y = pPlayer->getPosition().getY();
	pRect1->w = pPlayer->getWidth();
	pRect1->h = pPlayer->getHeight();

	for (unsigned int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->type() != std::string("Enemy") || !objects[i]->updating())
		{
			continue;
		}

		SDL_Rect* pRect2 = new SDL_Rect();
		pRect2->x = objects[i]->getPosition().getX();
		pRect2->y = objects[i]->getPosition().getY();
		pRect2->w = objects[i]->getWidth();
		pRect2->h = objects[i]->getHeight();

		if (AABBCollision(pRect1, pRect2))
		{
			if (!objects[i]->dead() && !objects[i]->dying())
			{
				pPlayer->collision();
			}
		}

		delete pRect2;
	}

	delete pRect1;
}

void CollisionManager::checkPlayerTileCollision(Player* pPlayer, const std::vector<TileLayer*>& collisionLayers)
{
	for (std::vector<TileLayer*>::const_iterator it = collisionLayers.begin(); it != collisionLayers.end(); ++it)
	{
		TileLayer* pTileLayer = (*it);
		std::vector<std::vector<int>> tiles = pTileLayer->getTileIDs();

		Vector2D layerPos = pTileLayer->getPosition();

		int x, y, tileColumn, tileRow, tileid = 0;

		x = layerPos.getX() / pTileLayer->getTileSize();
		y = layerPos.getY() / pTileLayer->getTileSize();

		if (pPlayer->getVelocity().getX() >= 0 || pPlayer->getVelocity().getY() >= 0)
		{
			tileColumn = ((pPlayer->getPosition().getX() + pPlayer->getWidth()) / pTileLayer->getTileSize());
			tileRow = ((pPlayer->getPosition().getY() + pPlayer->getHeight()) / pTileLayer->getTileSize());
			tileid = tiles[tileRow + y][tileColumn + x];
		}
		else if (pPlayer->getVelocity().getX() < 0 || pPlayer->getVelocity().getY() < 0)
		{
			tileColumn = (int)(pPlayer->getPosition().getX() / pTileLayer->getTileSize());
			tileRow = (int)(pPlayer->getPosition().getY() / pTileLayer->getTileSize());
			tileid = tiles[tileRow + y][tileColumn + x];
		}

		//pTileLayer[tileid].

		if (tileid != 0)
		{
			pPlayer->collision();
		}
	}
}

void CollisionManager::checkBulletTileCollision(const std::vector<TileLayer*>& collisionLayers)
{
	// are the any bullet been fired
	if (BulletHandler::Singleton()->getPlayerBullets().size() != 0 || BulletHandler::Singleton()->getEnemyBullets().size())
	{
		for (std::vector<TileLayer*>::const_iterator it = collisionLayers.begin(); it != collisionLayers.end(); ++it)
		{
			// first check the players 
			if (BulletHandler::Singleton()->getPlayerBullets().size() != 0)
			{
				for (int j = 0; j < BulletHandler::Singleton()->getPlayerBullets().size(); j++)
				{
					PlayerBullet* pPlayerBullet = BulletHandler::Singleton()->getPlayerBullets()[j];
					TileLayer* pTileLayer = (*it);
					std::vector<std::vector<int>> tiles = pTileLayer->getTileIDs();

					Vector2D layerPos = pTileLayer->getPosition();

					int x, y, tileColumn, tileRow, tileid = 0;

					x = ((Uint32)layerPos.getX() + Camera::Singleton()->getPosition().m_x) / pTileLayer->getTileSize();
					y = ((Uint32)layerPos.getY() - Camera::Singleton()->getPosition().m_y) / pTileLayer->getTileSize();

					if (pPlayerBullet->getVelocity().getX() >= 0 || pPlayerBullet->getVelocity().getY() >= 0)
					{
						tileColumn = ((pPlayerBullet->getPosition().getX() + pPlayerBullet->getWidth()) / pTileLayer->getTileSize());
						tileRow = ((pPlayerBullet->getPosition().getY() + pPlayerBullet->getHeight()) / pTileLayer->getTileSize());
						tileid = tiles[tileRow + y][tileColumn + x];
					}
					else if (pPlayerBullet->getVelocity().getX() < 0 || pPlayerBullet->getVelocity().getY() < 0)
					{
						tileColumn = pPlayerBullet->getPosition().getX() / pTileLayer->getTileSize();
						tileRow = pPlayerBullet->getPosition().getY() / pTileLayer->getTileSize();
						tileid = tiles[tileRow + y][tileColumn + x];
					}

					//pTileLayer[tileid].

					if (tileid != 0)
					{
						pPlayerBullet->collision();
					}
				}
			}
			// now check the enemies 
			if (BulletHandler::Singleton()->getEnemyBullets().size() != 0)
			{
				for (int j = 0; j < BulletHandler::Singleton()->getEnemyBullets().size(); j++)
				{
					EnemyBullet* pPlayerBullet = BulletHandler::Singleton()->getEnemyBullets()[j];
					TileLayer* pTileLayer = (*it);
					std::vector<std::vector<int>> tiles = pTileLayer->getTileIDs();

					Vector2D layerPos = pTileLayer->getPosition();

					int x, y, tileColumn, tileRow, tileid = 0;

					x = ((Uint32)layerPos.getX() - Camera::Singleton()->getPosition().m_x) / pTileLayer->getTileSize();
					y = ((Uint32)layerPos.getY() - Camera::Singleton()->getPosition().m_y) / pTileLayer->getTileSize();

					if (pPlayerBullet->getVelocity().getX() >= 0 || pPlayerBullet->getVelocity().getY() >= 0)
					{
						tileColumn = ((pPlayerBullet->getPosition().getX() + pPlayerBullet->getWidth()) / pTileLayer->getTileSize());
						tileRow = ((pPlayerBullet->getPosition().getY() + pPlayerBullet->getHeight()) / pTileLayer->getTileSize());
						tileid = tiles[tileRow + y][tileColumn + x];
					}
					else if (pPlayerBullet->getVelocity().getX() < 0 || pPlayerBullet->getVelocity().getY() < 0)
					{
						tileColumn = pPlayerBullet->getPosition().getX() / pTileLayer->getTileSize();
						tileRow = pPlayerBullet->getPosition().getY() / pTileLayer->getTileSize();
						tileid = tiles[tileRow + y][tileColumn + x];
					}

					//pTileLayer[tileid].

					if (tileid != 0)
					{
						pPlayerBullet->collision();
					}
				}
			}
		}
	}
}