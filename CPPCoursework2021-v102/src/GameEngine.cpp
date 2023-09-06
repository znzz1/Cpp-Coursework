#include "header.h"
#include "GameEngine.h"

int GameEngine::virtInitialiseObjects()
{
	return m_state->virtInitialiseObjects();
}

void GameEngine::virtRenderScreen(void)
{
	if (!m_bNeedsRedraw)
		return;

	m_pForegroundSurface->mySDLLockSurface();
	m_pBackgroundSurface->mySDLLockSurface();

	if (m_bTotalRedraw)
		copyAllBackgroundBuffer();
	else
		this->copyBackgroundPixels(m_iRedrawRealLeft, m_iRedrawRealTop, m_iRedrawRealRight - m_iRedrawRealLeft, m_iRedrawRealBottom - m_iRedrawRealTop);

	virtPreDraw();

	virtDrawStringsUnderneath();

	m_state->drawObjects();

	virtDrawStringsOnTop();

	virtPostDraw();

	m_pBackgroundSurface->mySDLUnlockSurface();
	m_pForegroundSurface->mySDLUnlockSurface();

	m_pForegroundSurface->mySDLUpdateTexture(m_pSDL2Texture);
	SDL_RenderCopy(m_pSDL2Renderer, m_pSDL2Texture, NULL, NULL);
	SDL_RenderPresent(m_pSDL2Renderer);

	m_bNeedsRedraw = false;
	m_bTotalRedraw = false;
	m_iRedrawVirtualLeft = 0xffffff;
	m_iRedrawRealLeft = 0xffffff;
	m_iRedrawVirtualTop = 0xffffff;
	m_iRedrawRealTop = 0xffffff;
	m_iRedrawVirtualRight = -0xffffff;
	m_iRedrawRealRight = -0xffffff;
	m_iRedrawVirtualBottom = -0xffffff;
	m_iRedrawRealBottom = -0xffffff;
}

void GameEngine::virtSetupBackgroundBuffer()
{
	m_state->virtSetupBackgroundBuffer();
}

void GameEngine::virtMainLoopDoBeforeUpdate()
{
	m_state->virtMainLoopDoBeforeUpdate();
}

void GameEngine::virtMainLoopDoAfterUpdate()
{
	m_state->virtMainLoopDoAfterUpdate();
}

void GameEngine::copyAllBackgroundBuffer()
{
	m_state->copyAllBackgroundBuffer();
}

void GameEngine::virtCreateSurfaces()
{
	BaseEngine::virtCreateSurfaces();
	m_state->virtCreateSurface();
}

void GameEngine::virtPreDraw()
{
	m_state->virtPreDraw();
}

void GameEngine::virtDrawStringsUnderneath()
{
	m_state->virtDrawStringsUnderneath();
}

void GameEngine::virtDrawStringsOnTop()
{
	m_state->virtDrawStringsOnTop();
}

void GameEngine::virtPostDraw()
{
	m_state->virtPostDraw();
}

void GameEngine::virtKeyUp(int iKeyCode)
{
	m_state->virtKeyUp(iKeyCode);
}

void GameEngine::virtKeyDown(int iKeyCode)
{
	m_state->virtKeyDown(iKeyCode);
}

void GameEngine::virtMouseMoved(int iX, int iY)
{
	m_state->virtMouseMoved(iX, iY);
}

void GameEngine::virtMouseUp(int iButton, int iX, int iY)
{
	m_state->virtMouseUp(iButton, iX, iY);
}

void GameEngine::virtMouseDown(int iButton, int iX, int iY)
{
	m_state->virtMouseDown(iButton, iX, iY);
}

void GameEngine::virtMouseWheel(int x, int y, int which, int timestamp)
{
	m_state->virtMouseWheel(x, y, which, timestamp);
}

void GameEngine::clearTime()
{
	m_iTick = 0;
	m_iTimeOffset = 0;
}

void GameEngine::initializeTiles()
{
	for (int i = 0; i < 25; i++)
		for (int j = 0; j < 18; j++)
			m_tileManager.setMapValue(i, j, rand());
	m_tileManager.setTopLeftPositionOnScreen(0, 0);
}