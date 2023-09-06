#ifndef BASEENEMYOBJECT_H
#define BASEENEMYOBJECT_H

#include "TemplateObject.h"

class GameTileManager;

class BaseEnemyObject : public TemplateObject
{
friend class GameState;
friend class UserObject;
protected:
	int enemyObjectId;

	bool dir;
	bool isDead;
	int enemyObjectHp;
	int hurtAnimationCount;

	int leftBoundary;
	int rightBoundary;

	int imageIndex;
public:
	BaseEnemyObject(int xStart, int yStart, BaseEngine* pEngine, int width, int height, int id, GameTileManager* gameTileManager, GameState* gameState, bool dir, bool isDead, int hp, int hurtAnimationCount, int imageIndex)
		: TemplateObject(xStart, yStart, pEngine, width, height, gameTileManager, gameState, true), 
		enemyObjectId(id), dir(dir), isDead(isDead), enemyObjectHp(hp), hurtAnimationCount(hurtAnimationCount), imageIndex(imageIndex)
	{
		getMovingBoundary();
	}

	virtual void virtDraw();
	virtual vector<vector<int>> getObjectPixelMap();

protected:
	void handleAttacks();
	virtual void getMovingBoundary();

private:
	vector<vector<int>> getBulletPixelMap();
};

#endif
