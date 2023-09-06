#include "header.h"
#include "GameEngine.h"

BaseState::~BaseState() {}

void BaseState::drawObjects() {}

void BaseState::virtSetupBackgroundBuffer() {}

int BaseState::virtInitialiseObjects() {return 0;}

void BaseState::virtMainLoopDoBeforeUpdate() {}

void BaseState::virtMainLoopDoAfterUpdate() {}

void BaseState::copyAllBackgroundBuffer() { engine->getForegroundSurface()->copyEntireSurface(engine->getBackgroundSurface()); }

void BaseState::virtCreateSurface() {}

void BaseState::virtPreDraw() {}

void BaseState::virtDrawStringsUnderneath() {}

void BaseState::virtDrawStringsOnTop() {}

void BaseState::virtPostDraw() {}

void BaseState::virtKeyUp(int iKeyCode) {}

void BaseState::virtKeyDown(int iKeyCode) {}

void BaseState::virtMouseMoved(int iX, int iY) {}

void BaseState::virtMouseUp(int iButton, int iX, int iY) {}

void BaseState::virtMouseDown(int iButton, int iX, int iY) {}

void BaseState::virtMouseWheel(int x, int y, int which, int timestamp) {}

