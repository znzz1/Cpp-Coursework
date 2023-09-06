#include "header.h"
#include "GameEngine.h"

WinState::WinState(GameEngine* engine)
	:BaseState(engine), offset(0)
{
	firework = ImageManager::loadImage("images/firework.png", true);
}

void WinState::virtSetupBackgroundBuffer()
{
	engine->getBackgroundSurface()->mySDLLockSurface();
	firework.renderImage(engine->getBackgroundSurface(), 0, firework.getHeight() - engine->getWindowHeight() - offset, 0, 0, engine->getWindowWidth(), engine->getWindowHeight());
	engine->getBackgroundSurface()->mySDLUnlockSurface();
}

void WinState::virtMainLoopDoBeforeUpdate()
{
	offset = (offset + 1) % (firework.getHeight() - engine->getWindowHeight());
	engine->redrawDisplay();
}

void WinState::copyAllBackgroundBuffer()
{
	engine->getBackgroundSurface()->mySDLLockSurface();
	firework.renderImage(engine->getBackgroundSurface(), 0, firework.getHeight() - engine->getWindowHeight() - offset, 0, 0, engine->getWindowWidth(), engine->getWindowHeight());
	engine->getBackgroundSurface()->mySDLUnlockSurface();

	engine->getForegroundSurface()->copyEntireSurface(engine->getBackgroundSurface());
}

void WinState::virtDrawStringsUnderneath()
{
	const char* congratulation = "YOU WIN!";
	engine->drawForegroundString(100, 316, congratulation, 0xcccccc, engine->getFont("resources/Bullpen3D.ttf", 88));

	const char* newGame = "START NEW GAME";
	engine->drawForegroundString(880, 250, newGame, 0xffff00, engine->getFont("resources/28 Days Later.ttf", 40));

	const char* upload = "UPLOAD YOUR SCORE";
	engine->drawForegroundString(850, 400, upload, 0xff00ff, engine->getFont("resources/28 Days Later.ttf", 40));

	const char* back = "BACK TO MAIN";
	engine->drawForegroundString(900, 550, back, 0x00ffff, engine->getFont("resources/28 Days Later.ttf", 40));
}

void WinState::virtMouseDown(int iButton, int iX, int iY)
{
	if (iButton == SDL_BUTTON_LEFT)
	{
		if (250 <= iY && iY <= 286 && 880 <= iX && iX <= 1166)
		{
			engine->setState(new GameState(engine));
			engine->virtSetupBackgroundBuffer();
			delete this;
		}
		else if (400 <= iY && iY <= 436 && 850 <= iX && iX <= 1190)
		{
			engine->setState(new UploadState(engine));
			engine->redrawDisplay();
			engine->virtSetupBackgroundBuffer();
			delete this;
		}
		else if (550 <= iY && iY <= 586 && 900 <= iX && iX <= 1132)
		{
			engine->setState(new InitialState(engine));
			engine->redrawDisplay();
			engine->virtCreateSurfaces();
			engine->virtSetupBackgroundBuffer();
			delete this;
		}
	}
}