#include "header.h"
#include "BaseEngine.h"
#include "BulletObject.h"
#include "GameTileManager.h"
#include "MyCollisionDetector.h"

void BulletObject::virtDraw()
{
	int imageIndex = rand() % 4;
	objectImages.getImage(imageIndex).renderImageWithMask(getEngine()->getForegroundSurface(), 0, 0,
		getDrawingRegionLeft(), getDrawingRegionTop(),
		m_iDrawWidth, m_iDrawHeight);
}

void BulletObject::virtDoUpdate(int iCurrentTime)
{
	int movement = dir ? 3 : -3;
	m_iCurrentScreenX += movement;

	if (m_iCurrentScreenX < 0 || m_iCurrentScreenX > getEngine()->getWindowWidth() - 1 - 30 || checkCollisionWithTiles())
		gameState->removeBulletObject(this);
}

void BulletObject::loadObjectImages()
{
	objectImages.setSize(4);
	objectImages.storeImage(0, ImageManager::loadImage("images/bullet_blue.png", true));
	objectImages.storeImage(1, ImageManager::loadImage("images/bullet_green.png", true));
	objectImages.storeImage(2, ImageManager::loadImage("images/bullet_red.png", true));
	objectImages.storeImage(3, ImageManager::loadImage("images/bullet_yellow.png", true));
}

vector<vector<int>> BulletObject::getObjectPixelMap()
{
	auto image = objectImages.getImage(0);

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

bool BulletObject::checkCollisionWithTiles()
{
	int mapX = dir ? gameTileManager->getMapXForScreenX(getDrawingRegionRight()) : gameTileManager->getMapXForScreenX(getDrawingRegionLeft());
	int mapY = gameTileManager->getMapYForScreenY(m_iCurrentScreenY);
	int offsetY = m_iCurrentScreenY % 40;

	if(gameTileManager->getMapValue(mapX, mapY) == 1 && MyCollisionDetector::checkCollision(getDrawingRegionLeft(), getDrawingRegionTop(), getObjectPixelMap(), mapX * 52, mapY * 40, gameState->getGameTilePixelMap()))
		return true;
	if (offsetY > 10)
	{
		if (gameTileManager->getMapValue(mapX, mapY + 1) == 1 && MyCollisionDetector::checkCollision(getDrawingRegionLeft(), getDrawingRegionTop(), getObjectPixelMap(), mapX * 52, (mapY + 1) * 40, gameState->getGameTilePixelMap()))
			return true;
	}
	return false;
}

ostream& operator<<(ostream& os, BulletObject& bullet)
{
	os << bullet.m_iCurrentScreenX << " " << bullet.m_iCurrentScreenY << " " << bullet.dir << "\n";
	return os;
}
