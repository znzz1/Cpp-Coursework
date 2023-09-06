#include "TileManager.h"
#include "ImageContainer.h"

class GameTileManager : public TileManager
{
private:
	int gameLevel;
	ImageContainer tileImages;
public:
	GameTileManager()
		: TileManager(40, 52, 25, 18)
	{
		loadTileImages();
	}

	virtual void virtDrawTileAt(BaseEngine* pEngine,
		DrawingSurface* pSurface,
		int iMapX, int iMapY,
		int iStartPositionScreenX, int iStartPositionScreenY) const override;

	void loadTileImages();

	void drawLine(BaseEngine* pEngine, DrawingSurface* pSurface, int iMapYLine, int iMapXStart, int drawNumber);
	void drawGamePortal(BaseEngine* pEngine, DrawingSurface* pSurface);
	void drawGamePrompt(BaseEngine* pEngine, DrawingSurface* pSurface);

	void setGameLevel(int level);
};

