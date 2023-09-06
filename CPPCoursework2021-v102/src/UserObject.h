#ifndef USEROBJECT_H
#define USEROBJECT_H

#include "TemplateObject.h"
#include "ImageContainer.h"
#include "GameStates.h"


class GameTileManager;


class UserObject : public TemplateObject
{
friend class GameState;
private:
	bool dir;
	bool isJumping;
	double verticalSpeed;
	int imageIndex;
	int attackCd;
public:
	UserObject(int xStart, int yStart, BaseEngine* pEngine, int width, int height, GameTileManager* gameTileManager, GameState* gameState, bool dir, bool jumping, double speed, int imageIndex, int attackCd)
		: TemplateObject(xStart, yStart, pEngine, width, height, gameTileManager, gameState, false),
		dir(dir), isJumping(jumping), verticalSpeed(speed), imageIndex(imageIndex), attackCd(attackCd)
	{
		loadObjectImages();
	}

	virtual void virtDraw();
	virtual void virtDoUpdate(int iCurrentTime);
	virtual void loadObjectImages();
	virtual vector<vector<int>> getObjectPixelMap();

	friend ostream& operator<<(ostream& os, UserObject& user);
private:
	int doubleToInt(double d);

	int calcUpCollisionDistance();
	int calcDownCollisionDistance();
	int calcLeftCollisionDistance();
	int calcRightCollisionDistance();
};

#endif