#include "header.h"
#include "BaseEngine.h"
#include "RegularEnemyObject.h"

void RegularEnemyObject::virtDoUpdate(int iCurrentTime)
{
	if (isDead)
		return;

	if (dir)
	{
		m_iCurrentScreenX += 2;
		if (m_iCurrentScreenX > rightBoundary)
		{
			m_iCurrentScreenX = rightBoundary;
			dir = false;
		}
	}
	else
	{
		m_iCurrentScreenX -= 2;
		if (m_iCurrentScreenX < leftBoundary)
		{
			m_iCurrentScreenX = leftBoundary;
			dir = true;
		}
	}

	handleAttacks();
}

void RegularEnemyObject::loadObjectImages()
{
	objectImages.setSize(6);
	objectImages.storeImage(0, ImageManager::loadImage("images/enemies/regular/stand_left.png", true));
	objectImages.storeImage(1, ImageManager::loadImage("images/enemies/regular/stand_right.png", true));
	objectImages.storeImage(2, ImageManager::loadImage("images/enemies/regular/hurt_left.png", true));
	objectImages.storeImage(3, ImageManager::loadImage("images/enemies/regular/hurt_right.png", true));
	objectImages.storeImage(4, ImageManager::loadImage("images/enemies/regular/death_left.png", true));
	objectImages.storeImage(5, ImageManager::loadImage("images/enemies/regular/death_right.png", true));
}

ostream& operator<<(ostream& os, RegularEnemyObject& enemy)
{
	os << enemy.m_iCurrentScreenX << " " << enemy.m_iCurrentScreenY << " " << enemy.dir << " " << enemy.isDead << " " << enemy.enemyObjectHp << " " << enemy.hurtAnimationCount << " " << enemy.imageIndex << "\n";
	return os;
}
