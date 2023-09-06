#include "header.h"
#include "GameTileManager.h"

void GameTileManager::virtDrawTileAt(BaseEngine* pEngine, DrawingSurface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const
{

	if (iMapY == 0) // Draw the upper boundary
	{
		tileImages.getImage(1).renderImage(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, 52, 40);
	}
	else if (iMapY == 17) // Draw the lower boundary
	{
		tileImages.getImage(0).renderImage(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, 52, 40);
	}
	else
		switch (gameLevel)
		{
		case 1: tileImages.getImage(2).renderImage(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, 52, 40); break;
		case 2: tileImages.getImage(3).renderImage(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, 52, 40); break;
		case 3: tileImages.getImage(4).renderImage(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, 52, 40); break;
		}
}

void GameTileManager::loadTileImages()
{
	tileImages.setSize(7);
	tileImages.storeImage(0, ImageManager::loadImage("images/ground.png", false));
	tileImages.storeImage(1, ImageManager::loadImage("images/cloud.png", false));
	tileImages.storeImage(2, ImageManager::loadImage("images/grass.png", false));
	tileImages.storeImage(3, ImageManager::loadImage("images/wood.png", false));
	tileImages.storeImage(4, ImageManager::loadImage("images/stone.png", false));
	tileImages.storeImage(5, ImageManager::loadImage("images/portal.png", false));
	tileImages.storeImage(6, ImageManager::loadImage("images/key_up.png", false));
}

void GameTileManager::drawLine(BaseEngine* pEngine, DrawingSurface* pSurface, int iMapYLine, int iMapXStart, int drawNumber)
{
	pSurface->mySDLLockSurface();
	for (int i = 0; i < drawNumber; i++)
			virtDrawTileAt(pEngine, pSurface, i + iMapXStart, iMapYLine, getTileWidth() * (i + iMapXStart), getTileHeight() * iMapYLine);
	pSurface->mySDLUnlockSurface();
}

void GameTileManager::drawGamePortal(BaseEngine* pEngine, DrawingSurface* pSurface)
{
	pSurface->mySDLLockSurface();
	switch (gameLevel)
	{
	case 1: tileImages.getImage(5).renderImageWithMask(pSurface, 0, 0, 1144, 80, 52, 40); break;
	case 2: tileImages.getImage(5).renderImageWithMask(pSurface, 0, 0, 1144, 80, 52, 40); break;
	case 3: tileImages.getImage(5).renderImageWithMask(pSurface, 0, 0, 1144, 80, 52, 40); break;
	}
	pSurface->mySDLUnlockSurface();
}

void GameTileManager::drawGamePrompt(BaseEngine* pEngine, DrawingSurface* pSurface)
{
	pSurface->mySDLLockSurface();
	tileImages.getImage(6).renderImageWithMask(pSurface, 0, 0, 1144, 40, 52, 40);
	pSurface->mySDLUnlockSurface();
}

void GameTileManager::setGameLevel(int level)
{
	this->gameLevel = level;
}
