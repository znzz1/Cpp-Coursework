#ifndef TEMPLATEOBJECT_H
#define TEMPLATEOBJECT_H

#include "DisplayableObject.h"
#include "ImageContainer.h"
#include "GameStates.h"
#include <iostream>

class GameTileManager;

class TemplateObject : public DisplayableObject
{
protected:
	ImageContainer objectImages;
	GameTileManager* gameTileManager;
	GameState* gameState;

public:
	TemplateObject(int xStart, int yStart, BaseEngine* pEngine, int width, int height, GameTileManager* gameTileManager, GameState* gameState, bool useTopLeftFor00)
		:DisplayableObject(xStart, yStart, pEngine, width, height, useTopLeftFor00), gameTileManager(gameTileManager), gameState(gameState)
	{}

	virtual void loadObjectImages() {}
	virtual vector<vector<int>> getObjectPixelMap() { return vector<vector<int>>(); }
};

#endif