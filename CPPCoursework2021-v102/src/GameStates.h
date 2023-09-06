#ifndef GAMESTATES_H
#define GAMESTATES_H

#include "DrawingSurface.h"
#include "MyDisplayableObjectContainer.h"
#include "ImageContainer.h"
#include "MyImagePixelMapping.h"
#include "MyFilterPoint.h"

using namespace std;


class GameEngine;
class SimpleImage;
class GameRecord;


class BaseState
{
protected:
	GameEngine* engine;
public:
	BaseState(GameEngine* engine)
		:engine(engine)
	{}

	virtual ~BaseState();

	virtual void drawObjects();
	virtual void virtSetupBackgroundBuffer();
	virtual int virtInitialiseObjects();	
	virtual void virtMainLoopDoBeforeUpdate();
	virtual void virtMainLoopDoAfterUpdate();
	virtual void copyAllBackgroundBuffer();
	virtual void virtCreateSurface();
	virtual void virtPreDraw();
	virtual void virtDrawStringsUnderneath();
	virtual void virtDrawStringsOnTop();
	virtual void virtPostDraw();
	virtual void virtKeyUp(int iKeyCode);
	virtual void virtKeyDown(int iKeyCode);
	virtual void virtMouseMoved(int iX, int iY);
	virtual void virtMouseUp(int iButton, int iX, int iY);
	virtual void virtMouseDown(int iButton, int iX, int iY);
	virtual void virtMouseWheel(int x, int y, int which, int timestamp);
};


class InitialState : public BaseState
{
private:
	unsigned int fontColor;
	int surfaceNumber;
	DrawingSurface wind;
	DrawingSurface fire;
	DrawingSurface water;
	DrawingSurface ice;
	DrawingSurface earth;
public:
	InitialState(GameEngine* engine);

	virtual void virtSetupBackgroundBuffer() override;
	virtual void virtMainLoopDoBeforeUpdate() override;
	virtual void virtCreateSurface() override;
	virtual void virtDrawStringsUnderneath() override;
	virtual void virtMouseDown(int iButton, int iX, int iY) override;
};


class GameState : public BaseState
{
friend class UserObject;
friend class BaseEnemyObject;
friend class RegularEnemyObject;
friend class EliteEnemyObject;
friend class BossEnemyObject;
friend class BulletObject;
private:
	ImageContainer gameImages;

	MyDisplayableObjectContainer user;
	MyDisplayableObjectContainer enemies;
	MyDisplayableObjectContainer bullets;

	int level;
	int score;
	int baseTime;
	int remainChances;
	int remainBullets;
	int remainEnemies;
public:
	GameState(GameEngine* engine, bool loadOld = false);
	~GameState();
	virtual void drawObjects() override;
	virtual void virtSetupBackgroundBuffer() override;
	virtual void virtMainLoopDoAfterUpdate() override;
	virtual void virtDrawStringsUnderneath() override;
	virtual void virtDrawStringsOnTop() override;
	virtual void virtKeyDown(int iKeyCode);
	virtual void virtMouseDown(int iButton, int iX, int iY) override;
	virtual void virtPreDraw() override;

	vector<vector<int>> getGameTilePixelMap();
	void removeBulletObject(DisplayableObject* bulletObject);
private:
	void saveGame();
	void saveGameState();

	void loadGameImages();
	void loadUserObject(bool loadOld);
	void saveUserObject();
	void loadBulletObjects(bool loadOld);
	void saveBulletObjects();

	void destoryAllObjects();

	void createMap();
	void createLevelOneMap();
	void createLevelTwoMap();
	void createLevelThreeMap();

	void loadEnemyObjects(bool loadOld);
	void saveEnemyObjects();
	void loadLevelOneEnemyObjects(bool loadOld);
	void saveLevelOneEnemyObjects();
	void loadLevelTwoEnemyObjects(bool loadOld);
	void saveLevelTwoEnemyObjects();
	void loadLevelThreeEnemyObjects(bool loadOld);
	void saveLevelThreeEnemyObjects();

	void drawUser();
	void updateUser(int iCurrentTime);
	void drawEnemies();
	void updateEnemies(int iCurrentTime);
	void drawBullets();
	void updateBullets(int iCurrentTime);

	void gameWin();
	void gameFail();
	void saveRecord();
};


class WinState : public BaseState
{
private:
	int offset;
	SimpleImage firework;
public:
	WinState(GameEngine* engine);

	virtual void virtSetupBackgroundBuffer() override;
	virtual void virtMainLoopDoBeforeUpdate() override;
	virtual void copyAllBackgroundBuffer() override;
	virtual void virtDrawStringsUnderneath() override;
	virtual void virtMouseDown(int iButton, int iX, int iY) override;
};


class LoseState : public BaseState
{
private:
	int offset;
	SimpleImage encourage;
public:
	LoseState(GameEngine* engine);

	virtual void virtSetupBackgroundBuffer() override;
	virtual void virtMainLoopDoBeforeUpdate() override;
	virtual void copyAllBackgroundBuffer() override;
	virtual void virtDrawStringsUnderneath() override;
	virtual void virtMouseDown(int iButton, int iX, int iY) override;
};


class HistoryState : public BaseState
{
private:
	vector<GameRecord> records;
	MyImagePixelMapping partialShift;
public:
	HistoryState(GameEngine* engine);
	~HistoryState();

	virtual void drawObjects() override;
	virtual void virtMainLoopDoBeforeUpdate() override;
	virtual void virtSetupBackgroundBuffer() override;
	virtual int virtInitialiseObjects() override;
	virtual void virtMouseDown(int iButton, int iX, int iY) override;
};


class GuidanceState : public BaseState
{
private:
	MyFilterPoint m_filter;
public:
	GuidanceState(GameEngine* engine);
	~GuidanceState();

	virtual void drawObjects() override;
	virtual void virtSetupBackgroundBuffer() override;
	virtual int virtInitialiseObjects() override;
	virtual void virtDrawStringsUnderneath() override;
	virtual void virtKeyDown(int iKeyCode) override;
	virtual void virtMouseWheel(int x, int y, int which, int timestamp) override;
};


class UploadState : public BaseState
{
private:
	string userName;
public:
	UploadState(GameEngine* engine);

	virtual void virtSetupBackgroundBuffer() override;
	virtual void virtDrawStringsUnderneath() override;
	virtual void virtKeyDown(int iKeyCode) override;
};

#endif