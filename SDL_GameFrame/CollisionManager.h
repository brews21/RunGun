#ifndef __CollisionManager__
#define __CollisionManager__

#include <iostream>
#include <vector>

class Player;
class GameObject;
class TileLayer;

class CollisionManager
{
public:
    void checkPlayerEnemyCollision(Player* pPlayer, const std::vector<GameObject*> &objects);
	void checkPlayerEnemyBulletCollision(Player* pPlayer);
	void checkEnemyPlayerBulletCollision(const std::vector<GameObject*>& objects);
	void checkPlayerTileCollision(Player* pPlayer, const std::vector<TileLayer*>& collisionLayers);
	void checkBulletTileCollision(const std::vector<TileLayer*>& collisionLayers);
};

#endif // __CollisionManager__
