#include <fstream>
#include "header.h"
#include "GameEngine.h"
#include "UserObject.h"
#include "BulletObject.h"
#include "RegularEnemyObject.h"
#include "EliteEnemyObject.h"
#include "BossEnemyObject.h"

#define LEVEL_COUNT  3
#define STATUS_BAR_HEIGHT 80
#define LEVEL_HEIGHT 640
#define TILE_HEIGHT 40

using namespace std;

GameState::GameState(GameEngine* engine, bool loadOld)
	:BaseState(engine)
{
	loadGameImages();
	user.createObjectArray(1);
	enemies.createObjectArray(6);

	ifstream gameInfo;

	if (!loadOld)
		gameInfo = ifstream("archive/new_game_state.txt");
	else
		gameInfo = ifstream("archive/old_game_state.txt");

	gameInfo >> level >> score >> baseTime >> remainChances >> remainBullets >> remainEnemies;
	engine->getTileManager()->setGameLevel(level);
	gameInfo.close();

	createMap();
	loadUserObject(loadOld);
	loadEnemyObjects(loadOld);
	loadBulletObjects(loadOld);

	engine->clearTime();

	if (loadOld)
		engine->pause();
}

GameState::~GameState()
{
	destoryAllObjects();
}

void GameState::drawObjects()
{
	drawEnemies();
	drawUser();
	drawBullets();
}

void GameState::virtSetupBackgroundBuffer()
{
	engine->fillBackground(0x000000);

	SimpleImage background = ImageManager::loadImage("images/mountain.png", true);
	engine->getBackgroundSurface()->mySDLLockSurface();
	background.renderImage(engine->getBackgroundSurface(), 0, LEVEL_HEIGHT * (LEVEL_COUNT - level), 0, TILE_HEIGHT, engine->getWindowWidth(), LEVEL_HEIGHT);
	engine->getBackgroundSurface()->mySDLUnlockSurface();

	createMap();
}

void GameState::virtMainLoopDoAfterUpdate()
{
	if (engine->isPaused())
		return;

	int time = engine->getModifiedTime();
	updateEnemies(time);
	updateUser(time);
	updateBullets(time);

	if (remainChances == 0)
		gameFail();
}

void GameState::virtDrawStringsUnderneath()
{
	const char* lives = "LIVES:";
	engine->drawForegroundString(50, 740, lives, 0xff0000, engine->getFont("resources/Diamond Fantasy.ttf", 40));

	auto image = gameImages.getImage(remainChances - 1);
	image.renderImageWithMask(engine->getForegroundSurface(), 0, 0, 180, 748, image.getWidth(), image.getHeight());

	char bullets[20];
	sprintf(bullets, "BULLETS: %d", remainBullets);
	engine->drawForegroundString(400, 740, bullets, 0x00ff00, engine->getFont("resources/Diamond Fantasy.ttf", 40));

	char level[10];
	sprintf(level, "LEVEL: %d", GameState::level);
	engine->drawForegroundString(750, 740, level, 0x0000ff, engine->getFont("resources/Diamond Fantasy.ttf", 40));

	char score[10];
	sprintf(score, "SCORE: %d", GameState::score);
	engine->drawForegroundString(1050, 740, score, 0xffffff, engine->getFont("resources/Diamond Fantasy.ttf", 40));
}

void GameState::virtDrawStringsOnTop()
{
	if (engine->isPaused())
	{
		auto image = gameImages.getImage(3);
		image.renderImageWithMask(engine->getForegroundSurface(), 0, 0, 425, 100, image.getWidth(), image.getHeight());

		char continueToPlay[16];
		sprintf(continueToPlay, "Continue To Play");
		engine->drawForegroundString(530, 300, continueToPlay, 0xffffff, engine->getFont("resources/Cute Rabbit.ttf", 48));

		char saveGame[9];
		sprintf(saveGame, "Save Game");
		engine->drawForegroundString(570, 400, saveGame, 0xffffff, engine->getFont("resources/Cute Rabbit.ttf", 48));

		char backToMain[12];
		sprintf(backToMain, "Back To Main");
		engine->drawForegroundString(550, 500, backToMain, 0xffffff, engine->getFont("resources/Cute Rabbit.ttf", 48));
	}
}

void GameState::virtKeyDown(int iKeyCode)
{
	if (iKeyCode == SDLK_UP && !engine->isPaused())
	{
		auto tileManager = engine->getTileManager();
		if (remainEnemies == 0 && tileManager->getMapXForScreenX(user.getDisplayableObject(0)->getXCentre()) == 22 && tileManager->getMapYForScreenY(user.getDisplayableObject(0)->getYCentre()) == 2)
		{
			if (level == 3)
			{
				gameWin();
			}
			else
			{
				engine->getTileManager()->setGameLevel(++level);
				virtSetupBackgroundBuffer();

				user.getDisplayableObject(0)->setPosition(650, 640);

				remainEnemies = 6;
				loadEnemyObjects(false);

				remainBullets = 5;
				bullets.destroyOldObjects(true);
				bullets.clearContents();
			}
		}
	}

	if (iKeyCode == SDLK_ESCAPE)
	{
		if (engine->isPaused())
			engine->unpause();
		else
			engine->pause();
	}

}

void GameState::virtMouseDown(int iButton, int iX, int iY)
{
	if (iButton == SDL_BUTTON_LEFT && engine->isPaused())
	{
		if (300 <= iY && iY <= 346 && 530 <= iX && iX <= 800)
			engine->unpause();
		else if (400 <= iY && iY <= 446 && 570 <= iX && iX <= 742)
			saveGame();
		else if (500 <= iY && iY <= 546 && 550 <= iX && iX <= 760)
		{
			destoryAllObjects();
			engine->setState(new InitialState(engine));
			engine->unpause();
			engine->virtCreateSurfaces();
			engine->virtSetupBackgroundBuffer();
			delete this;
		}
	}
}

void GameState::virtPreDraw()
{
	auto tileManager = engine->getTileManager();
	if (remainEnemies == 0)
		tileManager->drawGamePortal(engine, engine->getForegroundSurface());
	if (remainEnemies == 0 && tileManager->getMapXForScreenX(user.getDisplayableObject(0)->getXCentre()) == 22 && tileManager->getMapYForScreenY(user.getDisplayableObject(0)->getYCentre()) == 2)
		tileManager->drawGamePrompt(engine, engine->getForegroundSurface());

	if (level == 3 && remainEnemies <= 1)
		enemies.getDisplayableObject(5)->setVisible(true);
}

vector<vector<int>> GameState::getGameTilePixelMap()
{
	vector<vector<int>> res(engine->getTileManager()->getTileHeight(), vector<int>(engine->getTileManager()->getTileWidth(), 1));
	return res;
}

void GameState::removeBulletObject(DisplayableObject* bulletObject)
{
	bullets.removeDisplayableObject(bulletObject);
	delete bulletObject;
	remainBullets++;
}

void GameState::destoryAllObjects()
{
	user.destroyOldObjects(true);
	enemies.destroyOldObjects(true);
	bullets.destroyOldObjects(true);
}

void GameState::saveGame()
{	
	saveGameState();
	saveUserObject();
	saveEnemyObjects();
	saveBulletObjects();
}

void GameState::saveGameState()
{
	ofstream gameInfo;
	gameInfo.open("archive/old_game_state.txt", ios::trunc);
	gameInfo << level << " " << score << " " << baseTime + engine->getModifiedTime() << " " << remainChances << " " << remainBullets << " " << remainEnemies;
	gameInfo.close();
}

void GameState::loadGameImages()
{
	gameImages.setSize(4);
	gameImages.storeImage(0, ImageManager::loadImage("images/one_life.png", true));
	gameImages.storeImage(1, ImageManager::loadImage("images/two_lives.png", true));
	gameImages.storeImage(2, ImageManager::loadImage("images/three_lives.png", true));
	gameImages.storeImage(3, ImageManager::loadImage("images/pause_menu.png", true));
}

void GameState::loadBulletObjects(bool loadOld)
{
	if (!loadOld)
		return;

	int x;
	int y;
	bool dir;

	ifstream bulletInfo("archive/old_game_bullets.txt");
	for (int i = 0; i < 5 - remainBullets; i++)
	{
		bulletInfo >> x >> y >> dir;
		bullets.appendObjectToArray(new BulletObject(x, y, engine, dir, engine->getTileManager(), this));
	}
	bulletInfo.close();
	bullets.setAllObjectsVisible(true);
}
void GameState::saveBulletObjects()
{
	ofstream bulletInfo;
	bulletInfo.open("archive/old_game_bullets.txt");
	int i = 0;
	for (int i = 0; i < 5 - remainBullets; i++)
	{
		BulletObject* bulletObject = dynamic_cast<BulletObject*>(bullets.getDisplayableObject(i));
		bulletInfo << *bulletObject;
	}
	bulletInfo.close();
}

void GameState::loadUserObject(bool loadOld)
{
	ifstream userInfo;

	int userX, userY, imageIndex, attackCd;
	bool dir, jumping;
	double speed;

	if (!loadOld)
		userInfo = ifstream("archive/new_game_user.txt");
	else
		userInfo = ifstream("archive/old_game_user.txt");

	userInfo >> userX >> userY >> dir >> jumping >> speed >> imageIndex >> attackCd;
	userInfo.close();

	user.storeObjectInArray(0, new UserObject(userX, userY, engine, 26, 40, engine->getTileManager(), this, dir, jumping, speed, imageIndex, attackCd));
	user.setAllObjectsVisible(true);
}
void GameState::saveUserObject()
{
	ofstream userInfo;
	userInfo.open("archive/old_game_user.txt", ios::trunc);
	UserObject* userObject = dynamic_cast<UserObject*>(user.getDisplayableObject(0));
	userInfo << *userObject;
	userInfo.close();
}

void GameState::createMap()
{
	switch (level)
	{
	case 1: createLevelOneMap(); break;
	case 2: createLevelTwoMap(); break;
	case 3: createLevelThreeMap(); break;
	}
}
void GameState::createLevelOneMap()
{
	GameTileManager* tileManager = engine->getTileManager();

	const char* map[]{
		"1111111111111111111111111",
		"0000000000000000000000000",
		"0000000000000000000000000",
		"0000000000000000000011100",
		"0000000000000000000000000",
		"0000000000000000000000000",
		"0000000000111111111111111",
		"0000000000000000000000000",
		"0000000000000000000000000",
		"1111111100000000000000000",
		"0000000000000000000000000",
		"0000000000000000000000000",
		"0000000001111111000000000",
		"0000000000000000000000000",
		"0000000000000000000000000",
		"1111111100000000011111111",
		"0000000000000000000000000",
		"1111111111111111111111111"
	};

	for (int i = 0; i < 18; i++)
		for (int j = 0; j < 25; j++)
			tileManager->setMapValue(j, i, map[i][j] - '0');

	tileManager->drawLine(engine, engine->getBackgroundSurface(), 0, 0, 25);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 3, 20, 3);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 6, 10, 15);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 9, 0, 8);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 12, 9, 7);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 15, 0, 8);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 15, 17, 8);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 17, 0, 25);
}
void GameState::createLevelTwoMap()
{
	GameTileManager* tileManager = engine->getTileManager();

	const char* map[]{
	"1111111111111111111111111",
	"0000000000000000000000000",
	"0000000000000000000000000",
	"0000000000000000000011100",
	"0000000000000000000000000",
	"0000000000000000000000000",
	"0000000000111111111110000",
	"0000000000000000000000000",
	"0000001110000000000000000",
	"0000000000000000000000000",
	"0000000000000000000000000",
	"1111110000000000000000000",
	"0000000011100000000000000",
	"0000000000000111000000000",
	"0000000000000000000000000",
	"0000000111111000111111111",
	"0000000000000000000000000",
	"1111111111111111111111111"
	};

	for (int i = 0; i < 18; i++)
		for (int j = 0; j < 25; j++)
			tileManager->setMapValue(j, i, map[i][j] - '0');

	tileManager->drawLine(engine, engine->getBackgroundSurface(), 0, 0, 25);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 3, 20, 3);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 6, 10, 11);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 8, 6, 3);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 11, 0, 6);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 12, 8, 3);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 13, 13, 3);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 15, 7, 6);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 15, 16, 9);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 17, 0, 25);
}
void GameState::createLevelThreeMap()
{
	GameTileManager* tileManager = engine->getTileManager();

	const char* map[]{
	"1111111111111111111111111",
	"0000000000000000000000000",
	"0000000000000000000000000",
	"0000000000000000000011100",
	"0000000000000000000100000",
	"0000000000000000001000000",
	"0000000000000000010000000",
	"0001111111111111100000000",
	"0000000000000000000000000",
	"0000000000000000000000000",
	"1100000000000000000000011",
	"1111101100000000011011111",
	"0000000000000000000000000",
	"0000000001111111000000000",
	"0000000000000000000000000",
	"1111111000000000001111111",
	"1111111110000000111111111",
	"1111111111111111111111111"
	};

	for (int i = 0; i < 18; i++)
		for (int j = 0; j < 25; j++)
			tileManager->setMapValue(j, i, map[i][j] - '0');

	tileManager->drawLine(engine, engine->getBackgroundSurface(), 0, 0, 25);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 3, 20, 3);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 4, 19, 1);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 5, 18, 1);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 6, 17, 1);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 7, 3, 14);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 10, 0, 2);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 10, 23, 2);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 11, 0, 5);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 11, 6, 2);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 11, 17, 2);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 11, 20, 5);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 13, 9, 7);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 15, 0, 7);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 15, 18, 7);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 16, 0, 9);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 16, 16, 9);
	tileManager->drawLine(engine, engine->getBackgroundSurface(), 17, 0, 25);
}

void GameState::loadEnemyObjects(bool loadOld)
{
	switch (level)
	{
	case 1: loadLevelOneEnemyObjects(loadOld); break;
	case 2: loadLevelTwoEnemyObjects(loadOld); break;
	case 3: loadLevelThreeEnemyObjects(loadOld); break;
	}
}
void GameState::saveEnemyObjects()
{
	switch (level)
	{
	case 1: saveLevelOneEnemyObjects(); break;
	case 2: saveLevelTwoEnemyObjects(); break;
	case 3: saveLevelThreeEnemyObjects(); break;
	}
}

void GameState::loadLevelOneEnemyObjects(bool loadOld)
{
	int x1, x2, x3, x4, x5, x6, y1, y2, y3, y4, y5, y6;
	bool dir1, dir2, dir3, dir4, dir5, dir6;
	bool dead1, dead2, dead3, dead4, dead5, dead6;
	int hp1, hp2, hp3, hp4, hp5, hp6;
	int hurt1, hurt2, hurt3, hurt4, hurt5, hurt6;
	int image1, image2, image3, image4, image5, image6;
	
	ifstream enemiesInfo;
	if (!loadOld)
		enemiesInfo = ifstream("archive/new_game_enemies_one.txt");
	else
		enemiesInfo = ifstream("archive/old_game_enemies_one.txt");

	enemiesInfo >> x1 >> y1 >> dir1 >> dead1 >> hp1 >> hurt1 >> image1;
	enemiesInfo >> x2 >> y2 >> dir2 >> dead2 >> hp2 >> hurt2 >> image2;
	enemiesInfo >> x3 >> y3 >> dir3 >> dead3 >> hp3 >> hurt3 >> image3;
	enemiesInfo >> x4 >> y4 >> dir4 >> dead4 >> hp4 >> hurt4 >> image4;
	enemiesInfo >> x5 >> y5 >> dir5 >> dead5 >> hp5 >> hurt5 >> image5;
	enemiesInfo >> x6 >> y6 >> dir6 >> dead6 >> hp6 >> hurt6 >> image6;
	enemiesInfo.close();

	enemies.destroyOldObjects(true);
	enemies.storeObjectInArray(0, new RegularEnemyObject(x1, y1, engine, 0, engine->getTileManager(), this, dir1, dead1, hp1, hurt1, image1));
	enemies.storeObjectInArray(1, new RegularEnemyObject(x2, y2, engine, 1, engine->getTileManager(), this, dir2, dead2, hp2, hurt2, image2));
	enemies.storeObjectInArray(2, new RegularEnemyObject(x3, y3, engine, 2, engine->getTileManager(), this, dir3, dead3, hp3, hurt3, image3));
	enemies.storeObjectInArray(3, new RegularEnemyObject(x4, y4, engine, 3, engine->getTileManager(), this, dir4, dead4, hp4, hurt4, image4));
	enemies.storeObjectInArray(4, new RegularEnemyObject(x5, y5, engine, 4, engine->getTileManager(), this, dir5, dead5, hp5, hurt5, image5));
	enemies.storeObjectInArray(5, new RegularEnemyObject(x6, y6, engine, 5, engine->getTileManager(), this, dir6, dead6, hp6, hurt6, image6));
}
void GameState::saveLevelOneEnemyObjects()
{
	ofstream enemyInfo;
	enemyInfo.open("archive/old_game_enemies_one.txt", ios::trunc);
	for (int i = 0; i < 6; i++)
	{
		RegularEnemyObject* enemyObject = dynamic_cast<RegularEnemyObject*>(enemies.getDisplayableObject(i));
		enemyInfo << *enemyObject;
	}
	enemyInfo.close();
}


void GameState::loadLevelTwoEnemyObjects(bool loadOld)
{
	int x1, x2, x3, x4, x5, x6, y1, y2, y3, y4, y5, y6;
	bool dir1, dir2, dir3, dir4, dir5, dir6;
	bool dead1, dead2, dead3, dead4, dead5, dead6;
	int hp1, hp2, hp3, hp4, hp5, hp6;
	int hurt1, hurt2, hurt3, hurt4, hurt5, hurt6;
	int image1, image2, image3, image4, image5, image6;
	int speed5, speed6;

	ifstream enemiesInfo;
	if (!loadOld)
		enemiesInfo = ifstream("archive/new_game_enemies_two.txt");
	else
		enemiesInfo = ifstream("archive/old_game_enemies_two.txt");

	enemiesInfo >> x1 >> y1 >> dir1 >> dead1 >> hp1 >> hurt1 >> image1;
	enemiesInfo >> x2 >> y2 >> dir2 >> dead2 >> hp2 >> hurt2 >> image2;
	enemiesInfo >> x3 >> y3 >> dir3 >> dead3 >> hp3 >> hurt3 >> image3;
	enemiesInfo >> x4 >> y4 >> dir4 >> dead4 >> hp4 >> hurt4 >> image4;
	enemiesInfo >> x5 >> y5 >> dir5 >> dead5 >> hp5 >> hurt5 >> image5 >> speed5;
	enemiesInfo >> x6 >> y6 >> dir6 >> dead6 >> hp6 >> hurt6 >> image6 >> speed6;
	enemiesInfo.close();

	enemies.destroyOldObjects(true);
	enemies.storeObjectInArray(0, new RegularEnemyObject(x1, y1, engine, 0, engine->getTileManager(), this, dir1, dead1, hp1, hurt1, image1));
	enemies.storeObjectInArray(1, new RegularEnemyObject(x2, y2, engine, 1, engine->getTileManager(), this, dir2, dead2, hp2, hurt2, image2));
	enemies.storeObjectInArray(2, new RegularEnemyObject(x3, y3, engine, 2, engine->getTileManager(), this, dir3, dead3, hp3, hurt3, image3));
	enemies.storeObjectInArray(3, new RegularEnemyObject(x4, y4, engine, 3, engine->getTileManager(), this, dir4, dead4, hp4, hurt4, image4));
	enemies.storeObjectInArray(4, new EliteEnemyObject(x5, y5, engine, 4, engine->getTileManager(), this, dir5, dead5, hp5, hurt5, image5, speed5));
	enemies.storeObjectInArray(5, new EliteEnemyObject(x6, y6, engine, 5, engine->getTileManager(), this, dir6, dead6, hp6, hurt6, image6, speed6));
}
void GameState::saveLevelTwoEnemyObjects()
{
	ofstream enemyInfo;
	enemyInfo.open("archive/old_game_enemies_two.txt", ios::trunc);
	for (int i = 0; i < 4; i++)
	{
		RegularEnemyObject* enemyObject = dynamic_cast<RegularEnemyObject*>(enemies.getDisplayableObject(i));
		enemyInfo << *enemyObject;
	}
	for (int i = 4; i < 6; i++)
	{
		EliteEnemyObject* enemyObject = dynamic_cast<EliteEnemyObject*>(enemies.getDisplayableObject(i));
		enemyInfo << *enemyObject;
	}
	enemyInfo.close();
}

void GameState::loadLevelThreeEnemyObjects(bool loadOld)
{
	int x1, x2, x3, x4, x5, x6, y1, y2, y3, y4, y5, y6;
	bool dir1, dir2, dir3, dir4, dir5, dir6;
	bool dead1, dead2, dead3, dead4, dead5, dead6;
	int hp1, hp2, hp3, hp4, hp5, hp6;
	int hurt1, hurt2, hurt3, hurt4, hurt5, hurt6;
	int image1, image2, image3, image4, image5, image6;
	int speed1, speed2, speed3, speed4, speed5;
	int pos6, time6, attack6;

	ifstream enemiesInfo;
	if (!loadOld)
		enemiesInfo = ifstream("archive/new_game_enemies_three.txt");
	else
		enemiesInfo = ifstream("archive/old_game_enemies_three.txt");

	enemiesInfo >> x1 >> y1 >> dir1 >> dead1 >> hp1 >> hurt1 >> image1 >> speed1;
	enemiesInfo >> x2 >> y2 >> dir2 >> dead2 >> hp2 >> hurt2 >> image2 >> speed2;
	enemiesInfo >> x3 >> y3 >> dir3 >> dead3 >> hp3 >> hurt3 >> image3 >> speed3;
	enemiesInfo >> x4 >> y4 >> dir4 >> dead4 >> hp4 >> hurt4 >> image4 >> speed4;
	enemiesInfo >> x5 >> y5 >> dir5 >> dead5 >> hp5 >> hurt5 >> image5 >> speed5;
	enemiesInfo >> x6 >> y6 >> dir6 >> dead6 >> hp6 >> hurt6 >> image6 >> pos6 >> time6 >> attack6;
	enemiesInfo.close();

	enemies.destroyOldObjects(true);
	enemies.storeObjectInArray(0, new EliteEnemyObject(x1, y1, engine, 0, engine->getTileManager(), this, dir1, dead1, hp1, hurt1, image1, speed1));
	enemies.storeObjectInArray(1, new EliteEnemyObject(x2, y2, engine, 1, engine->getTileManager(), this, dir2, dead2, hp2, hurt2, image2, speed2));
	enemies.storeObjectInArray(2, new EliteEnemyObject(x3, y3, engine, 2, engine->getTileManager(), this, dir3, dead3, hp3, hurt3, image3, speed3));
	enemies.storeObjectInArray(3, new EliteEnemyObject(x4, y4, engine, 3, engine->getTileManager(), this, dir4, dead4, hp4, hurt4, image4, speed4));
	enemies.storeObjectInArray(4, new EliteEnemyObject(x5, y5, engine, 4, engine->getTileManager(), this, dir5, dead5, hp5, hurt5, image5, speed5));
	enemies.storeObjectInArray(5, new BossEnemyObject(x6, y6, engine, 5, engine->getTileManager(), this, dir6, dead6, hp6, hurt6, image6, pos6, time6, attack6));
}
void GameState::saveLevelThreeEnemyObjects()
{
	ofstream enemyInfo;
	enemyInfo.open("archive/old_game_enemies_three.txt", ios::trunc);
	for (int i = 0; i < 5; i++)
	{
		EliteEnemyObject* enemyObject = dynamic_cast<EliteEnemyObject*>(enemies.getDisplayableObject(i));
		enemyInfo << *enemyObject;
	}
	BossEnemyObject* enemyObject = dynamic_cast<BossEnemyObject*>(enemies.getDisplayableObject(5));
	enemyInfo << *enemyObject;

	enemyInfo.close();
}


void GameState::drawUser()
{
	for (auto it = user.m_vecDisplayableObjects.begin(); it != user.m_vecDisplayableObjects.end(); it++)
		if (*it)
			(*it)->setHasActed(false);

	bool finished = false;
	while (!finished)
	{
		user.m_bDrawableObjectsChanged = false;
		finished = true;
		for (auto it = user.m_vecDisplayableObjects.begin(); it != user.m_vecDisplayableObjects.end(); it++)
		{
			if (*it)
			{
				if (!(*it)->getHasActed())
				{
					(*it)->setHasActed(true);
					(*it)->virtDraw();
				}
				if (user.m_bDrawableObjectsChanged)
				{
					finished = false;
					break;
				}
			}
		}
	}
}
void GameState::updateUser(int iCurrentTime)
{
	for (auto it = user.m_vecDisplayableObjects.begin(); it != user.m_vecDisplayableObjects.end(); it++)
		if (*it)
			(*it)->setHasActed(false);

	bool finished = false;
	while (!finished)
	{
		user.m_bDrawableObjectsChanged = false;
		finished = true;
		for (auto it = user.m_vecDisplayableObjects.begin(); it != user.m_vecDisplayableObjects.end(); it++)
		{
			if (*it)
			{
				if (!(*it)->getHasActed())
				{
					(*it)->setHasActed(true);
					(*it)->virtDoUpdate(iCurrentTime + baseTime);
				}
				if (user.m_bDrawableObjectsChanged)
				{
					finished = false;

				}
			}
		}
	}
}
void GameState::drawEnemies()
{
	for (auto it = enemies.m_vecDisplayableObjects.begin(); it != enemies.m_vecDisplayableObjects.end(); it++)
		if (*it)
			(*it)->setHasActed(false);

	bool finished = false;
	while (!finished)
	{
		enemies.m_bDrawableObjectsChanged = false;
		finished = true;
		for (auto it = enemies.m_vecDisplayableObjects.begin(); it != enemies.m_vecDisplayableObjects.end(); it++)
		{
			if (*it)
			{
				if (!(*it)->getHasActed())
				{
					(*it)->setHasActed(true);
					(*it)->virtDraw();
				}
				if (enemies.m_bDrawableObjectsChanged)
				{
					finished = false;
					break;
				}
			}
		}
	}
}
void GameState::updateEnemies(int iCurrentTime)
{
	for (auto it = enemies.m_vecDisplayableObjects.begin(); it !=  enemies.m_vecDisplayableObjects.end(); it++)
		if (*it)
			(*it)->setHasActed(false);

	bool finished = false;
	while (!finished)
	{
		enemies.m_bDrawableObjectsChanged = false;
		finished = true;
		for (auto it = enemies.m_vecDisplayableObjects.begin(); it != enemies.m_vecDisplayableObjects.end(); it++)
		{
			if (*it)
			{
				if (!(*it)->getHasActed())
				{
					(*it)->setHasActed(true);
					(*it)->virtDoUpdate(iCurrentTime + baseTime);
				}
				if (enemies.m_bDrawableObjectsChanged)
				{
					finished = false;
					break;
				}
			}
		}
	}
}
void GameState::drawBullets()
{
	for (auto it = bullets.m_vecDisplayableObjects.begin(); it != bullets.m_vecDisplayableObjects.end(); it++)
		if (*it)
			(*it)->setHasActed(false);

	bool finished = false;
	while (!finished)
	{
		bullets.m_bDrawableObjectsChanged = false;
		finished = true;
		for (auto it = bullets.m_vecDisplayableObjects.begin(); it != bullets.m_vecDisplayableObjects.end(); it++)
		{
			if (*it)
			{
				if (!(*it)->getHasActed())
				{
					(*it)->setHasActed(true);
					(*it)->virtDraw();
				}
				if (bullets.m_bDrawableObjectsChanged)
				{
					finished = false;
					break;
				}
			}
		}
	}
}
void GameState::updateBullets(int iCurrentTime)
{
	for (auto it = bullets.m_vecDisplayableObjects.begin(); it != bullets.m_vecDisplayableObjects.end(); it++)
		if (*it)
			(*it)->setHasActed(false);

	bool finished = false;
	while (!finished)
	{
		bullets.m_bDrawableObjectsChanged = false;
		finished = true;
		for (auto it = bullets.m_vecDisplayableObjects.begin(); it != bullets.m_vecDisplayableObjects.end(); it++)
		{
			if (*it)
			{
				if (!(*it)->getHasActed())
				{
					(*it)->setHasActed(true);
					(*it)->virtDoUpdate(iCurrentTime + baseTime);
				}
				if (bullets.m_bDrawableObjectsChanged)
				{
					finished = false;
					break;
				}
			}
		}
	}
}

void GameState::gameWin()
{
	saveRecord();
	destoryAllObjects();

	engine->setState(new WinState(engine));
	engine->virtSetupBackgroundBuffer();

	delete this;
}
void GameState::gameFail()
{
	saveRecord();
	destoryAllObjects();

	engine->setState(new LoseState(engine));
	engine->virtSetupBackgroundBuffer();

	delete this;
}
void GameState::saveRecord()
{
	ofstream record;
	record.open("archive/current_game_state.txt", ios::trunc);
	record << score << " " << baseTime + engine->getModifiedTime();
	record.close();
}