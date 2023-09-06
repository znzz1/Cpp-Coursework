#include "BaseEnemyObject.h"

#define ELITE_ENEMY_WIDTH 48
#define ELITE_ENEMY_HEIGHT 40

class EliteEnemyObject : public BaseEnemyObject
{
private:
	int ySpeed;
public:
	EliteEnemyObject(int xStart, int yStart, BaseEngine* pEngine, int id, GameTileManager* gameTileManager, GameState* gameState, bool dir, bool isDead, int hp, int hurtAnimationCount, int imageIndex, int ySpeed)
		: BaseEnemyObject(xStart, yStart, pEngine, ELITE_ENEMY_WIDTH, ELITE_ENEMY_HEIGHT, id, gameTileManager, gameState, dir, isDead, hp, hurtAnimationCount, imageIndex), ySpeed(ySpeed)
	{
		loadObjectImages();
	}

	virtual void virtDoUpdate(int iCurrentTime);
	virtual void loadObjectImages();
	friend ostream& operator<<(ostream& os, EliteEnemyObject& enemy);
};

