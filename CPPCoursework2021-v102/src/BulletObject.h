#ifndef BULLETOBJECT_H
#define BULLETOBJECT_H

#include "TemplateObject.h"
#include "ImageContainer.h"
#include "GameStates.h"

#define BULLET_HEIGHT 30
#define BULLET_WIDTH 30

class GameTileManager;

class BulletObject : public TemplateObject
{
friend class GameState;
friend class BossEnemyObject;
private:
    bool dir;

public:
    BulletObject(int xStart, int yStart, BaseEngine* pEngine, bool dir, GameTileManager* gameTileManager, GameState* gameState)
        :TemplateObject(xStart, yStart, pEngine, BULLET_WIDTH, BULLET_HEIGHT, gameTileManager, gameState, true), dir(dir)
    {
        loadObjectImages();
    }

    virtual void virtDraw();
    virtual void virtDoUpdate(int iCurrentTime);
    virtual void loadObjectImages();
    virtual vector<vector<int>>	getObjectPixelMap();
    friend ostream& operator<<(ostream& os, BulletObject& bullet);
private:
    bool checkCollisionWithTiles();
};

#endif