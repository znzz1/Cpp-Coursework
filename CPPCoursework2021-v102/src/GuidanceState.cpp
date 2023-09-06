#include "header.h"
#include "GameEngine.h"
#include "ExampleDragableObjects.h"

GuidanceState::GuidanceState(GameEngine* engine)
	:BaseState(engine), m_filter(0, 0, 0, 0, engine)
{
	engine->getBackgroundSurface()->setDrawPointsFilter(&m_filter);
	engine->getForegroundSurface()->setDrawPointsFilter(&m_filter);
	virtInitialiseObjects();
	virtSetupBackgroundBuffer();
	engine->redrawDisplay();
}

GuidanceState::~GuidanceState()
{
	engine->destroyOldObjects(true);
}

void GuidanceState::drawObjects()
{
	engine->BaseEngine::drawAllObjects();
}

void GuidanceState::virtSetupBackgroundBuffer()
{
	engine->getBackgroundSurface()->mySDLLockSurface();
	SimpleImage image = ImageManager::loadImage("images/guidance.png", true);
	image.renderImage(engine->getBackgroundSurface(), 0, 0, 0, 0, image.getWidth(), image.getHeight());
	engine->getBackgroundSurface()->mySDLUnlockSurface();
}

int GuidanceState::virtInitialiseObjects()
{
	engine->drawableObjectsChanged();
	engine->destroyOldObjects(true);
	engine->createObjectArray(5);

	engine->storeObjectInArray(0, new DragableImageObject(550, 230, engine, "images/left.jpg", false));
	engine->storeObjectInArray(1, new DragableImageObject(650, 230, engine, "images/right.jpg", false));
	engine->storeObjectInArray(2, new DragableImageObject(600, 378, engine, "images/up.jpg", false));
	engine->storeObjectInArray(3, new DragableImageObject(600, 528, engine, "images/space.png", false));
	engine->storeObjectInArray(4, new DragableImageObject(600, 675, engine, "images/s.jpg", false));

	engine->setAllObjectsVisible(true);
	return 0;
}

void GuidanceState::virtDrawStringsUnderneath()
{
	const char* guidance = "Game Guidance";
	engine->drawForegroundString(800, 50, guidance, 0xffffff, engine->getFont("resources/Diamond Fantasy.ttf", 56));

	const char* prompt1 = "Left and Right key to move";
	engine->drawForegroundString(700, 200, prompt1, 0x000000, engine->getFont("resources/Bullpen3D.ttf", 40));

	const char* prompt2 = "Up key to enter the portal";
	engine->drawForegroundString(700, 350, prompt2, 0x000000, engine->getFont("resources/Bullpen3D.ttf", 40));

	const char* prompt3 = "Space key to jump";
	engine->drawForegroundString(700, 500, prompt3, 0x000000, engine->getFont("resources/Bullpen3D.ttf", 40));

	const char* prompt4 = "S key to shoot the bullet";
	engine->drawForegroundString(700, 650, prompt4, 0x000000, engine->getFont("resources/Bullpen3D.ttf", 40));
}

void GuidanceState::virtKeyDown(int iKeyCode)
{
	switch (iKeyCode)
	{
	case SDLK_ESCAPE:
		engine->getBackgroundSurface()->setDrawPointsFilter(engine);
		engine->getForegroundSurface()->setDrawPointsFilter(engine);
		engine->setState(new InitialState(engine));
		engine->redrawDisplay();
		engine->virtCreateSurfaces();
		engine->virtSetupBackgroundBuffer();
		delete this;
		break;
	case SDLK_LEFT:
		m_filter.changeOffset(5, 0);
		engine->redrawDisplay();
		break;
	case SDLK_RIGHT:
		m_filter.changeOffset(-5, 0);
		engine->redrawDisplay();
		break;
	case SDLK_UP:
		m_filter.changeOffset(0, 5);
		engine->redrawDisplay();
		break;
	case SDLK_DOWN:
		m_filter.changeOffset(0, -5);
		engine->redrawDisplay();
		break;
	case SDLK_SPACE:
		m_filter.setOffset(0, 0);
		engine->redrawDisplay();
		break;
	}
}

void GuidanceState::virtMouseWheel(int x, int y, int which, int timestamp)
{
	int iOldCentreX = engine->convertClickedToVirtualPixelXPosition(engine->getWindowWidth() / 2);
	int iOldCentreY = engine->convertClickedToVirtualPixelYPosition(engine->getWindowHeight() / 2);

	if (y < 0)
		m_filter.compress();
	else if (y > 0)
		m_filter.stretch();

	int iNewCentreX = engine->convertClickedToVirtualPixelXPosition(engine->getWindowWidth() / 2);
	int iNewCentreY = engine->convertClickedToVirtualPixelYPosition(engine->getWindowHeight() / 2);

	engine->redrawDisplay();
}
