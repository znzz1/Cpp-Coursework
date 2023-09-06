#include "header.h"
#include "BossEnemyObject.h"
#include "GameTileManager.h"
#include "UserObject.h"
#include "MyCollisionDetector.h"
#include "BulletObject.h"
#include <queue>
#include <vector>

struct Position {
	int x;
	int y;
};

struct Bullet {
	int x;
	int y;
	bool dir;
};

void BossEnemyObject::virtDraw()
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
		{
			if (attackable == true)
				imageIndex = 0;
			else
				imageIndex = 6;
		}
		else
		{
			if (attackable == true)
				imageIndex = 1;
			else
				imageIndex = 7;
		}
	}

	auto image = objectImages.getImage(imageIndex);
	image.renderImageWithMask(getEngine()->getForegroundSurface(), 0, 0,
		m_iCurrentScreenX, m_iCurrentScreenY + m_iDrawHeight - image.getHeight(), m_iDrawWidth, image.getHeight());
}

void BossEnemyObject::virtDoUpdate(int iCurrentTime)
{
	if (isDead || m_bVisible == false)
		return;
	
	if (traceTime == 0)
	{
		nextPos = findNextPos();
		if (!nextPos)
			traceTime = 2;
		else
			traceTime = 5;
	}
	else
		traceTime--;

	if (!predictAttacksInThreeFrame())
	{
		switch (nextPos)
		{
		case STOP: break;
		case UP: m_iCurrentScreenY -= 3; break;
		case DOWN: m_iCurrentScreenY += 3; break;
		case LEFT: dir = false; m_iCurrentScreenX -= 3; break;
		case RIGHT: dir = true; m_iCurrentScreenX += 3; break;
		}
	}
	else
	{
		switch (nextPos)
		{
		case STOP: break;
		case UP: m_iCurrentScreenY += 3; break;
		case DOWN: m_iCurrentScreenY -= 3; break;
		case LEFT: 
			dir = true; 
			m_iCurrentScreenX += 3; 
			m_iCurrentScreenY += 3;
			break;
		case RIGHT: 
			dir = false; 
			m_iCurrentScreenX -= 3;
			m_iCurrentScreenY += 3;
			break;
		}
	}

	if (!isInTile())
	{
		attackable = true;
		handleAttacks();
	}
	else
	{
		attackable = false;
	}
}

void BossEnemyObject::loadObjectImages()
{
	objectImages.setSize(8);
	objectImages.storeImage(0, ImageManager::loadImage("images/enemies/boss/stand_left.png", true));
	objectImages.storeImage(1, ImageManager::loadImage("images/enemies/boss/stand_right.png", true));
	objectImages.storeImage(2, ImageManager::loadImage("images/enemies/boss/hurt_left.png", true));
	objectImages.storeImage(3, ImageManager::loadImage("images/enemies/boss/hurt_right.png", true));
	objectImages.storeImage(4, ImageManager::loadImage("images/enemies/boss/death_left.png", true));
	objectImages.storeImage(5, ImageManager::loadImage("images/enemies/boss/death_right.png", true));
	objectImages.storeImage(6, ImageManager::loadImage("images/enemies/boss/stand_inTile_left.png", true));
	objectImages.storeImage(7, ImageManager::loadImage("images/enemies/boss/stand_inTile_right.png", true));
}

int BossEnemyObject::findNextPos()
{
	bool find = false;

	int xStart = gameTileManager->getMapXForScreenX(m_iCurrentScreenX + m_iDrawWidth / 2);
	int yStart = gameTileManager->getMapYForScreenY(m_iCurrentScreenY + m_iDrawHeight / 2);

	if (gameTileManager->getMapValue(xStart, yStart))
	{
		xStart = gameTileManager->getMapXForScreenX(m_iCurrentScreenX);
		yStart = gameTileManager->getMapYForScreenY(m_iCurrentScreenY);
	}

	UserObject* user = dynamic_cast<UserObject*>(gameState->user.getDisplayableObject(0));
	int xTar = gameTileManager->getMapXForScreenX(user->getDrawingRegionLeft() + user->getDrawWidth() / 2);
	int yTar = gameTileManager->getMapYForScreenY(user->getDrawingRegionTop() + user->getDrawHeight() / 2);

	if (xStart == xTar && yStart == yTar)
	{
		if (user->getDrawingRegionLeft() > m_iCurrentScreenX)
			return RIGHT;
		return LEFT;
	}

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
	int vis[25][18];
	memset(vis, -1, sizeof(vis));

	static const int move_x[] = { 0, 0, -1, 1 };
	static const int move_y[] = { 1, -1, 0, 0 };

	Position path[25][18];

	queue<Position> p;
	p.push({ xStart, yStart });
	vis[xStart][yStart] = 0;

	// BFS
	while (!p.empty())
	{
		Position now = p.front();
		p.pop();
		if (now.x == xTar && now.y == yTar)
		{
			find = true;
			break;
		}
		for (int i = 0; i < 4; i++)
		{
			int next_x = now.x + move_x[i];
			int next_y = now.y + move_y[i];
			if (next_x >= 0 && next_x < 25 && next_y >= 0 && next_y < 18 && vis[next_x][next_y] == -1 && map[next_y][next_x] != '1')
			{
				p.push({ next_x, next_y });
				vis[next_x][next_y] = 0;
				path[next_x][next_y] = now;
			}
		}
	}
	
	if (!find)
		return STOP;

	// Backwards the path to get the next position
	int xTmp = xTar, yTmp = yTar;
	while (true)
	{
		Position pos = path[xTmp][yTmp];
		if (pos.x == xStart && pos.y == yStart)
			break;
		xTmp = pos.x;
		yTmp = pos.y;
	}

	if (xTmp > xStart)
		return RIGHT;

	if (xTmp < xStart)
		return LEFT;

	if (yTmp > yStart)
		return DOWN;

	return UP;
}

bool BossEnemyObject::isInTile()
{
	int mapX = gameTileManager->getMapXForScreenX(m_iCurrentScreenX);
	int mapY = gameTileManager->getMapYForScreenY(m_iCurrentScreenY);

	if(gameTileManager->getMapValue(mapX, mapY) == 1)
		return true;

	if (m_iCurrentScreenX % 52 > 12 && gameTileManager->getMapValue(mapX + 1, mapY) == 1)
		return true;

	if (m_iCurrentScreenY % 40 != 0 && (gameTileManager->getMapValue(mapX, mapY + 1) || gameTileManager->getMapValue(mapX + 1, mapY + 1)))
		return true;

	return false;
}

bool BossEnemyObject::predictAttacksInThreeFrame()
{
	vector<Bullet> bullets;

	int virtX = m_iCurrentScreenX;
	int virtY = m_iCurrentScreenY;
	bool preDir = dir;

	for (int i = 0; i < 5 - gameState->remainBullets; i++)
	{
		BulletObject* bullet = dynamic_cast<BulletObject*>(gameState->bullets.getDisplayableObject(i));
		bullets.push_back({ bullet->m_iCurrentScreenX, bullet->m_iCurrentScreenY, bullet->dir });
	}

	for (int i = 0; i < 3; i++)
	{
		switch (nextPos)
		{
		case STOP: break;
		case UP: virtY -= 3; break;
		case DOWN: virtY += 3; break;
		case LEFT: dir = false; virtX -= 2; break;
		case RIGHT: dir = true; virtX += 2; break;
		}

		for (auto i = bullets.begin(); i != bullets.end(); i++)
		{
			if ((*i).dir)
				(*i).x += 4;
			else
				(*i).x -= 4;

			if (MyCollisionDetector::checkCollision(virtX, virtY, getObjectPixelMap(), (*i).x, (*i).y, gameState->getGameTilePixelMap()))
			{
				dir = preDir;
				return true;
			}
		}
	}
	dir = preDir;
	return false;
}

ostream& operator<<(ostream& os, BossEnemyObject& enemy)
{
	os << enemy.m_iCurrentScreenX << " " << enemy.m_iCurrentScreenY << " " << enemy.dir << " " << enemy.isDead << " " << enemy.enemyObjectHp << " " << enemy.hurtAnimationCount << " " << enemy.imageIndex << " " << enemy.nextPos << " " << enemy.traceTime << " " << enemy.attackable << "\n";
	return os;
}
