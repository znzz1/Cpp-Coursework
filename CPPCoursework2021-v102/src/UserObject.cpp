#include "header.h"
#include "BaseEngine.h"
#include "UserObject.h"
#include "GameTileManager.h"
#include "MyCollisionDetector.h"
#include "BulletObject.h"
#include "BaseEnemyObject.h"

#define GA 0.1875

void UserObject::virtDraw()
{
	if (dir == false)
		if (!isJumping)
			imageIndex = 0;
		else
			if (verticalSpeed < 0)
				imageIndex = 2;
			else
				imageIndex = 4;
	else
		if (!isJumping)
			imageIndex = 1;
		else
			if (verticalSpeed < 0)
				imageIndex = 3;
			else
				imageIndex = 5;

	objectImages.getImage(imageIndex).renderImageWithMask(getEngine()->getForegroundSurface(), 0, 0,
		getDrawingRegionLeft(), getDrawingRegionTop(),
		m_iDrawWidth, m_iDrawHeight);
}

void UserObject::virtDoUpdate(int iCurrentTime)
{
	if (getEngine()->isKeyPressed(SDLK_s) && gameState->remainBullets > 0)
	{
		if (attackCd == 0)
		{
			(gameState->remainBullets)--;
			int x = dir ? m_iCurrentScreenX + 20 : m_iCurrentScreenX - 50;
			int y = m_iCurrentScreenY - 15;
			gameState->bullets.appendObjectToArray(new BulletObject(x, y, getEngine(), dir, gameTileManager, gameState));
			attackCd = 10;
		}
		else
		{
			attackCd--;
		}
	}

	// Simulate the physical engine
	if (getEngine()->isKeyPressed(SDLK_LEFT) && getEngine()->isKeyPressed(SDLK_RIGHT))
	{
	}
	else if (getEngine()->isKeyPressed(SDLK_LEFT))
	{
		dir = false;
		m_iCurrentScreenX -= 3;
		if (m_iCurrentScreenX < m_iDrawWidth / 2)
			m_iCurrentScreenX = m_iDrawWidth / 2;
	}
	else if (getEngine()->isKeyPressed(SDLK_RIGHT))
	{
		dir = true;
		m_iCurrentScreenX += 3;
		if (m_iCurrentScreenX > m_pEngine->getWindowWidth() - m_iDrawWidth / 2)
			m_iCurrentScreenX = m_pEngine->getWindowWidth() - m_iDrawWidth / 2;
	}

	int offset = calcLeftCollisionDistance();
	if (offset > 0)
		m_iCurrentScreenX += offset;
	else
		m_iCurrentScreenX -= calcRightCollisionDistance();

	if (getEngine()->isKeyPressed(SDLK_SPACE) && !isJumping)
	{
		isJumping = true;
		verticalSpeed = -6.6875;
	}

	m_iCurrentScreenY += doubleToInt(verticalSpeed);

	if (verticalSpeed < 0)
	{
		int offset = calcUpCollisionDistance();
		m_iCurrentScreenY += offset;

		if (offset > 0)
			verticalSpeed = 0;
		else
			verticalSpeed += GA;
	}
	else if (verticalSpeed > 0)
	{
		int offset = calcDownCollisionDistance();
		m_iCurrentScreenY -= offset;

		if (offset > 0)
		{
			isJumping = false;
			verticalSpeed = 0;
		}
		else
			verticalSpeed += GA;
	}
	else
		verticalSpeed = 1;

	// Detect collision with enemies
	for (int i = 0; i < 6; i++)
	{
		BaseEnemyObject* enemy = dynamic_cast<BaseEnemyObject*>(gameState->enemies.getDisplayableObject(i));
		if (enemy != nullptr && enemy->isDead == false && enemy->m_bVisible == true)
		{
			if (MyCollisionDetector::checkCollision(getDrawingRegionLeft(), getDrawingRegionTop(), getObjectPixelMap(),
				enemy->getDrawingRegionLeft(), enemy->getDrawingRegionTop(), enemy->getObjectPixelMap()))
			{
				gameState->remainChances--;
				setPosition(650, 640);
				break;
			}
		}
	}

	this->redrawDisplay();
}

void UserObject::loadObjectImages()
{
	objectImages.setSize(6);
	objectImages.storeImage(0, ImageManager::loadImage("images/stand_left.png", false));
	objectImages.storeImage(1, ImageManager::loadImage("images/stand_right.png", false));
	objectImages.storeImage(2, ImageManager::loadImage("images/jump_left.png", false));
	objectImages.storeImage(3, ImageManager::loadImage("images/jump_right.png", false));
	objectImages.storeImage(4, ImageManager::loadImage("images/land_left.png", false));
	objectImages.storeImage(5, ImageManager::loadImage("images/land_right.png", false));
}

vector<vector<int>> UserObject::getObjectPixelMap()
{
	auto image = objectImages.getImage(imageIndex);

	int y = image.getHeight();
	int x = image.getWidth();

	vector<vector<int>> res(y, vector<int>(x, 0));

	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++)
		{
			res[i][j] = (image.getPixelColour(j, i) > 0) ? 1 : 0;
		}
	}

	return res;
}

int UserObject::doubleToInt(double d)
{
	return int(d - 0.5);
}

int UserObject::calcUpCollisionDistance()
{
	int res = 0;

	int offsetY = m_iCurrentScreenY % 40;
	if (offsetY < 20)
	{
		int offsetX = m_iCurrentScreenX % 52;

		int mapX = gameTileManager->getMapXForScreenX(m_iCurrentScreenX);
		int mapY = gameTileManager->getMapYForScreenY(m_iCurrentScreenY);

		if (gameTileManager->getMapValue(mapX, mapY - 1) == 1)
		{
			res = MyCollisionDetector::calcCollisionDistance(getDrawingRegionLeft(), getDrawingRegionTop(), getObjectPixelMap(),
				mapX * 52, (mapY -1) * 40, gameState->getGameTilePixelMap(), 3);
		}

		if (offsetX < 13 && gameTileManager->getMapValue(mapX - 1, mapY - 1) == 1)
		{
			int tmp = MyCollisionDetector::calcCollisionDistance(getDrawingRegionLeft(), getDrawingRegionTop(), getObjectPixelMap(),
				(mapX - 1) * 52, (mapY - 1) * 40, gameState->getGameTilePixelMap(), 3);
			res = tmp > res ? tmp : res;
		}

		if (offsetX > 39 && gameTileManager->getMapValue(mapX + 1, mapY - 1) == 1)
		{
			int tmp = MyCollisionDetector::calcCollisionDistance(getDrawingRegionLeft(), getDrawingRegionTop(), getObjectPixelMap(),
				(mapX + 1) * 52, (mapY - 1) * 40, gameState->getGameTilePixelMap(), 3);
			res = tmp > res ? tmp : res;
		}
	}
	return res;
}
int UserObject::calcDownCollisionDistance()
{
	int res = 0;

	int offsetY = m_iCurrentScreenY % 40;
	if (offsetY > 20)
	{
		int offsetX = m_iCurrentScreenX % 52;

		int mapX = gameTileManager->getMapXForScreenX(m_iCurrentScreenX);
		int mapY = gameTileManager->getMapYForScreenY(m_iCurrentScreenY);

		if (gameTileManager->getMapValue(mapX, mapY + 1) == 1)
		{
			res = MyCollisionDetector::calcCollisionDistance(getDrawingRegionLeft(), getDrawingRegionTop(), getObjectPixelMap(),
				mapX * 52, (mapY + 1) * 40, gameState->getGameTilePixelMap(), 4);
		}

		if (offsetX < 13 && gameTileManager->getMapValue(mapX - 1, mapY + 1) == 1)
		{
			int tmp = MyCollisionDetector::calcCollisionDistance(getDrawingRegionLeft(), getDrawingRegionTop(), getObjectPixelMap(),
				(mapX - 1) * 52, (mapY + 1) * 40, gameState->getGameTilePixelMap(), 4);
			res = tmp > res ? tmp : res;
		}

		if (offsetX > 39 && gameTileManager->getMapValue(mapX + 1, mapY + 1) == 1)
		{
			int tmp = MyCollisionDetector::calcCollisionDistance(getDrawingRegionLeft(), getDrawingRegionTop(), getObjectPixelMap(),
				(mapX + 1) * 52, (mapY + 1) * 40, gameState->getGameTilePixelMap(), 4);
			res = tmp > res ? tmp : res;
		}
	}
	return res;
}
int UserObject::calcLeftCollisionDistance()
{
	int res = 0;

	int offsetX = m_iCurrentScreenX % 52;
	if (offsetX < 13)
	{
		int offsetY = m_iCurrentScreenY % 40;

		int mapX = gameTileManager->getMapXForScreenX(m_iCurrentScreenX);
		int mapY = gameTileManager->getMapYForScreenY(m_iCurrentScreenY);

		if (gameTileManager->getMapValue(mapX - 1, mapY) == 1)
		{
			res = MyCollisionDetector::calcCollisionDistance(getDrawingRegionLeft(), getDrawingRegionTop(), getObjectPixelMap(),
				(mapX - 1)*52, mapY * 40, gameState->getGameTilePixelMap(), 1);
		}

		if (offsetY > 20 && gameTileManager->getMapValue(mapX - 1, mapY + 1) == 1)
		{
			int tmp = MyCollisionDetector::calcCollisionDistance(getDrawingRegionLeft(), getDrawingRegionTop(), getObjectPixelMap(),
				(mapX - 1) * 52, (mapY + 1) * 40, gameState->getGameTilePixelMap(), 1);
			res = tmp > res ? tmp : res;
		}

		if (offsetY < 20 && gameTileManager->getMapValue(mapX - 1, mapY - 1) == 1)
		{
			int tmp = MyCollisionDetector::calcCollisionDistance(getDrawingRegionLeft(), getDrawingRegionTop(), getObjectPixelMap(),
				(mapX - 1) * 52, (mapY - 1 * 40), gameState->getGameTilePixelMap(), 1);
			res = tmp > res ? tmp : res;
		}
	}
	return res;
}
int UserObject::calcRightCollisionDistance()
{
	int res = 0;

	int offsetX = m_iCurrentScreenX % 52;
	if (offsetX > 39)
	{
		int offsetY = m_iCurrentScreenY % 40;

		int mapX = gameTileManager->getMapXForScreenX(m_iCurrentScreenX);
		int mapY = gameTileManager->getMapYForScreenY(m_iCurrentScreenY);

		if (gameTileManager->getMapValue(mapX + 1, mapY) == 1)
		{
			res = MyCollisionDetector::calcCollisionDistance(getDrawingRegionLeft(), getDrawingRegionTop(), getObjectPixelMap(),
				(mapX + 1) * 52, mapY * 40, gameState->getGameTilePixelMap(), 2);
		}

		if (offsetY > 20 && gameTileManager->getMapValue(mapX + 1, mapY + 1) == 1)
		{
			int tmp = MyCollisionDetector::calcCollisionDistance(getDrawingRegionLeft(), getDrawingRegionTop(), getObjectPixelMap(),
				(mapX + 1) * 52, (mapY + 1) * 40, gameState->getGameTilePixelMap(), 2);
			res = tmp > res ? tmp : res;
		}

		if (offsetY < 20 && gameTileManager->getMapValue(mapX + 1, mapY - 1) == 1)
		{
			int tmp = MyCollisionDetector::calcCollisionDistance(getDrawingRegionLeft(), getDrawingRegionTop(), getObjectPixelMap(),
				(mapX + 1) * 52, (mapY - 1 * 40), gameState->getGameTilePixelMap(), 2);
			res = tmp > res ? tmp : res;
		}
	}
	return res;
}

ostream& operator<<(ostream& os, UserObject& user)
{
	os << user.m_iCurrentScreenX << " " << user.m_iCurrentScreenY << " " << user.dir << " " << user.isJumping << " " << user.verticalSpeed << " " << user.imageIndex << " " << user.attackCd;
	return os;
}
