#include "header.h"
#include "BaseEngine.h"
#include "BaseEnemyObject.h"
#include "MyCollisionDetector.h"
#include "GameTileManager.h"
#include "BulletObject.h"

void BaseEnemyObject::virtDraw() 
{
	if (m_bVisible == false)
		return;

	if (isDead)
	{
		if (dir == false)
			imageIndex = 4;
		else
			imageIndex = 5;
	}
	else if (hurtAnimationCount != 0)
	{
		hurtAnimationCount--;
		if (dir == false)
			imageIndex = 2;
		else
			imageIndex = 3;
	}
	else
	{
		if (dir == false)
			imageIndex = 0;
		else
			imageIndex = 1;
	}

	auto image = objectImages.getImage(imageIndex);
	image.renderImageWithMask(getEngine()->getForegroundSurface(), 0, 0,
		m_iCurrentScreenX, m_iCurrentScreenY + m_iDrawHeight - image.getHeight(), m_iDrawWidth, image.getHeight());
}

vector<vector<int>> BaseEnemyObject::getObjectPixelMap()
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

void BaseEnemyObject::handleAttacks()
{
	for (int i = 0; i < 5 - gameState->remainBullets; i++)
	{
		BulletObject* bullet = dynamic_cast<BulletObject*>(gameState->bullets.getDisplayableObject(i));
		if (MyCollisionDetector::checkCollision(getDrawingRegionLeft(), getDrawingRegionTop(), getObjectPixelMap(),
			bullet->getDrawingRegionLeft(), bullet->getDrawingRegionTop(), bullet->getObjectPixelMap()))
		{
			hurtAnimationCount = 20;
			enemyObjectHp--;
			if (enemyObjectHp == 0)
			{
				isDead = true;
				gameState->remainEnemies--;
				gameState->score++;
			}
			gameState->removeBulletObject(bullet);
		}
	}
}

void BaseEnemyObject::getMovingBoundary()
{
	int x = gameTileManager->getMapXForScreenX(m_iCurrentScreenX);
	int y = gameTileManager->getMapYForScreenY(m_iCurrentScreenY);
	while (gameTileManager->getMapValue(x, y) == 0)
		y++;

	int l = x;
	while (l > 0)
	{
		l--;
		if (gameTileManager->getMapValue(l, y) == 0)
		{
			l++;
			break;
		}
	}

	int r = x;
	while (r < gameTileManager->getMapWidth())
	{
		r++;
		if (gameTileManager->getMapValue(r, y) == 0)
			break;
	}

	leftBoundary = l * gameTileManager->getTileWidth();
	rightBoundary = r * gameTileManager->getTileWidth() - m_iDrawWidth;
}

vector<vector<int>> BaseEnemyObject::getBulletPixelMap()
{
	SimpleImage image = ImageManager::loadImage("images/bullet_blue.png", true);

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
