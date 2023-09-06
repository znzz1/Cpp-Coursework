#include "BaseEngine.h"
#include "GameStates.h"
#include "GameTileManager.h"

class GameEngine :	public BaseEngine
{
private:
	BaseState* m_state;
	GameTileManager m_tileManager;

public:
	GameEngine()
		: BaseEngine()
	{
		m_state = new InitialState(this);
		//m_state = new GuidanceState(this);

		initializeTiles();
	}

	virtual ~GameEngine() 
	{
		delete m_state;
	}

	inline void setState(BaseState* state) { m_state = state; }
	inline GameTileManager* getTileManager() { return &m_tileManager; }
	
	virtual int virtInitialiseObjects();
	virtual void virtRenderScreen(void);

	virtual void virtSetupBackgroundBuffer();
	virtual void virtMainLoopDoBeforeUpdate();
	virtual void virtMainLoopDoAfterUpdate();
	virtual void copyAllBackgroundBuffer();
	virtual void virtCreateSurfaces();

	virtual void virtPreDraw();
	virtual void virtDrawStringsUnderneath();
	virtual void virtDrawStringsOnTop();
	virtual void virtPostDraw();

	virtual void virtKeyUp(int iKeyCode);
	virtual void virtKeyDown(int iKeyCode);
	virtual void virtMouseMoved(int iX, int iY);
	virtual void virtMouseUp(int iButton, int iX, int iY);
	virtual void virtMouseDown(int iButton, int iX, int iY);
	virtual void virtMouseWheel(int x, int y, int which, int timestamp);

	void clearTime();
private:
	void initializeTiles();
};



