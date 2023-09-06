#include "header.h"
#include "EliteEnemyObject.h"

void EliteEnemyObject::virtDoUpdate(int iCurrentTime)
{
	if (isDead && ySpeed == -8)
		return;

	m_iCurrentScreenY += ySpeed;

	if (ySpeed == 8)
		ySpeed = -8;
	else
		ySpeed++;

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

void EliteEnemyObject::loadObjectImages()
{
	objectImages.setSize(6);
	objectImages.storeImage(0, ImageManager::loadImage("images/enemies/elite/stand_left.png", true));
	objectImages.storeImage(1, ImageManager::loadImage("images/enemies/elite/stand_right.png", true));
	objectImages.storeImage(2, ImageManager::loadImage("images/enemies/elite/hurt_left.png", true));
	objectImages.storeImage(3, ImageManager::loadImage("images/enemies/elite/hurt_right.png", true));
	objectImages.storeImage(4, ImageManager::loadImage("images/enemies/elite/death_left.png", true));
	objectImages.storeImage(5, ImageManager::loadImage("images/enemies/elite/death_right.png", true));
}

ostream& operator<<(ostream& os, EliteEnemyObject& enemy)
{
	os << enemy.m_iCurrentScreenX << " " << enemy.m_iCurrentScreenY << " " << enemy.dir << " " << enemy.isDead << " " << enemy.enemyObjectHp << " " << enemy.hurtAnimationCount << " " << enemy.imageIndex << " " << enemy.ySpeed << "\n";
	return os;
}
