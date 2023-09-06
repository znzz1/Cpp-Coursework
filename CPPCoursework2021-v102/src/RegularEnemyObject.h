#include "BaseEnemyObject.h"

#define REG_ENEMY_WIDTH 40
#define REG_ENEMY_HEIGHT 64

class RegularEnemyObject : public BaseEnemyObject
{
public:
	RegularEnemyObject(int xStart, int yStart, BaseEngine* pEngine, int id, GameTileManager* gameTileManager, GameState* gameState, bool dir, bool isDead, int hp, int hurtAnimationCount, int imageIndex)
		: BaseEnemyObject(xStart, yStart, pEngine, REG_ENEMY_WIDTH, REG_ENEMY_HEIGHT, id, gameTileManager, gameState, dir, isDead, hp, hurtAnimationCount, imageIndex)
	{
		loadObjectImages();
	}

	virtual void virtDoUpdate(int iCurrentTime);
	virtual void loadObjectImages();
	friend ostream& operator<<(ostream& os, RegularEnemyObject& enemy);
};