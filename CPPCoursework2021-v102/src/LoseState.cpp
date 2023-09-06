#include "header.h"
#include "GameEngine.h"

LoseState::LoseState(GameEngine* engine)
	:BaseState(engine), offset(0)
{
	encourage = ImageManager::loadImage("images/encourage.png", true);
}

void LoseState::virtSetupBackgroundBuffer()
{
	engine->getBackgroundSurface()->mySDLLockSurface();
	encourage.renderImage(engine->getBackgroundSurface(), 0, encourage.getHeight() - engine->getWindowHeight() - offset, 0, 0, engine->getWindowWidth(), engine->getWindowHeight());
	engine->getBackgroundSurface()->mySDLUnlockSurface();
}

void LoseState::virtMainLoopDoBeforeUpdate()
{
	offset = (offset + 1) % (encourage.getHeight() - engine->getWindowHeight());
	engine->redrawDisplay();
}

void LoseState::copyAllBackgroundBuffer()
{
	engine->getBackgroundSurface()->mySDLLockSurface();
	encourage.renderImage(engine->getBackgroundSurface(), 0, encourage.getHeight() - engine->getWindowHeight() - offset, 0, 0, engine->getWindowWidth(), engine->getWindowHeight());
	engine->getBackgroundSurface()->mySDLUnlockSurface();

	engine->getForegroundSurface()->copyEntireSurface(engine->getBackgroundSurface());
}

void LoseState::virtDrawStringsUnderneath()
{
	const char* fail = "YOU LOSE!";
	engine->drawForegroundString(100, 316, fail, 0x111111, engine->getFont("resources/Bullpen3D.ttf", 88));

	const char* newGame = "START NEW GAME";
	engine->drawForegroundString(880, 250, newGame, 0x000000, engine->getFont("resources/Cute Rabbit.ttf", 44));

	const char* upload = "UPLOAD YOUR SCORE";
	engine->drawForegroundString(860, 400, upload, 0x000000, engine->getFont("resources/Cute Rabbit.ttf", 44));

	const char* back = "BACK TO MAIN";
	engine->drawForegroundString(900, 550, back, 0x000000, engine->getFont("resources/Cute Rabbit.ttf", 44));
}

void LoseState::virtMouseDown(int iButton, int iX, int iY)
{
	if (iButton == SDL_BUTTON_LEFT)
	{
		if (250 <= iY && iY <= 290 && 880 <= iX && iX <= 1116)
		{
			engine->setState(new GameState(engine));
			engine->virtSetupBackgroundBuffer();
			delete this;
		}
		else if (400 <= iY && iY <= 440 && 860 <= iX && iX <= 1135)
		{
			engine->setState(new UploadState(engine));
			engine->redrawDisplay();
			engine->virtSetupBackgroundBuffer();
			delete this;
		}
		else if (550 <= iY && iY <= 590 && 900 <= iX && iX <= 1092)
		{
			engine->setState(new InitialState(engine));
			engine->redrawDisplay();
			engine->virtCreateSurfaces();
			engine->virtSetupBackgroundBuffer();
			delete this;
		}
	}
}