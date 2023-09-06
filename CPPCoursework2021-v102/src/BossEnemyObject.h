#include "BaseEnemyObject.h"

#define BOSS_ENEMY_WIDTH 40
#define BOSS_ENEMY_HEIGHT 40

#define STOP 0
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

class BossEnemyObject : public BaseEnemyObject
{
public:
	BossEnemyObject(int xStart, int yStart, BaseEngine* pEngine, int id, GameTileManager* gameTileManager, GameState* gameState, bool dir, bool isDead, int hp, int hurtAnimationCount, int imageIndex, int nextPos, int traceTime, bool attackable)
		: BaseEnemyObject(xStart, yStart, pEngine, BOSS_ENEMY_WIDTH, BOSS_ENEMY_HEIGHT, id, gameTileManager, gameState, dir, isDead, hp, hurtAnimationCount, imageIndex), nextPos(nextPos), traceTime(traceTime), attackable(attackable)
	{
		loadObjectImages();
		m_bVisible = false;
	}

	virtual void virtDraw();
	virtual void virtDoUpdate(int iCurrentTime);
	virtual void loadObjectImages();
	friend ostream& operator<<(ostream& os, BossEnemyObject& enemy);

private:
	int findNextPos();
	bool isInTile();
	bool predictAttacksInThreeFrame();
private:
	int nextPos;
	int traceTime;
	bool attackable;
};

