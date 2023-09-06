#include "header.h"
#include "GameEngine.h"

#define WIND 0x77c59c
#define FIRE 0xf48d24
#define WATER 0x185792
#define ICE 0x4a9aca
#define EARTH 0x484540

InitialState::InitialState(GameEngine* engine)
	:BaseState(engine), 
	wind(engine), fire(engine), water(engine), ice(engine), earth(engine), 
	surfaceNumber(0), fontColor(WIND)
{ }

void InitialState::virtSetupBackgroundBuffer()
{
	// Load all background images
	SimpleImage i_wind = ImageManager::loadImage("images/wind_slime.png", true);
	SimpleImage i_fire = ImageManager::loadImage("images/fire_slime.png", true);
	SimpleImage i_water = ImageManager::loadImage("images/water_slime.png", true);
	SimpleImage i_ice = ImageManager::loadImage("images/ice_slime.png", true);
	SimpleImage i_earth = ImageManager::loadImage("images/earth_slime.png", true);

	// Copy the images to corresponding surfaces
	wind.mySDLLockSurface();
	i_wind.renderImageBlit(engine, &wind, 
		0, 0, engine->getWindowWidth(), engine->getWindowHeight(), 
		0, 0, i_wind.getWidth(), i_wind.getHeight());
	wind.mySDLUnlockSurface();

	fire.mySDLLockSurface();
	i_fire.renderImageBlit(engine, &fire,
		0, 0, engine->getWindowWidth(), engine->getWindowHeight(),
		0, 0, i_fire.getWidth(), i_fire.getHeight());
	fire.mySDLUnlockSurface();

	water.mySDLLockSurface();
	i_water.renderImageBlit(engine, &water,
		0, 0, engine->getWindowWidth(), engine->getWindowHeight(),
		0, 0, i_water.getWidth(), i_water.getHeight());
	water.mySDLUnlockSurface();

	ice.mySDLLockSurface();
	i_ice.renderImageBlit(engine, &ice,
		0, 0, engine->getWindowWidth(), engine->getWindowHeight(),
		0, 0, i_ice.getWidth(), i_ice.getHeight());
	ice.mySDLUnlockSurface();

	earth.mySDLLockSurface();
	i_earth.renderImageBlit(engine, &earth,
		0, 0, engine->getWindowWidth(), engine->getWindowHeight(),
		0, 0, i_earth.getWidth(), i_earth.getHeight());
	earth.mySDLUnlockSurface();
}

void InitialState::virtMainLoopDoBeforeUpdate()
{
	surfaceNumber = (surfaceNumber + 1) % 800;

	if (surfaceNumber <= 160)
	{
		engine->getBackgroundSurface()->copyEntireSurface(&wind);
		fontColor = WIND;
	}
	else if (surfaceNumber <= 320) {
		engine->getBackgroundSurface()->copyEntireSurface(&fire);
		fontColor = FIRE;
	}
	else if (surfaceNumber <= 480)
	{
		engine->getBackgroundSurface()->copyEntireSurface(&water);
		fontColor = WATER;
	}
	else if (surfaceNumber <= 640)
	{
		engine->getBackgroundSurface()->copyEntireSurface(&ice);
		fontColor = ICE;
	}
	else
	{
		engine->getBackgroundSurface()->copyEntireSurface(&earth);
		fontColor = EARTH;
	}

	engine->redrawDisplay();	
}

void InitialState::virtCreateSurface()
{
	wind.createSurface(engine->getWindowWidth(), engine->getWindowHeight());
	fire.createSurface(engine->getWindowWidth(), engine->getWindowHeight());
	water.createSurface(engine->getWindowWidth(), engine->getWindowHeight());
	ice.createSurface(engine->getWindowWidth(), engine->getWindowHeight());
	earth.createSurface(engine->getWindowWidth(), engine->getWindowHeight());
}

void InitialState::virtDrawStringsUnderneath()
{
	// 756
	const char* title = "ESCAPE THE ENEMY WORLD";
	engine->drawForegroundString(130, 590, title, 0xdddddd, engine->getFont("resources/28 Days Later.ttf", 80));


	// 208 36
	const char* start = "NEW JOURNEY";
	engine->drawForegroundString(996, 230, start, fontColor, engine->getFont("resources/Cute Rabbit.ttf", 48));

	// 170 36
	const char* load = "LOAD GAME";
	engine->drawForegroundString(1015, 320, load, fontColor, engine->getFont("resources/Cute Rabbit.ttf", 48));

	// 146 36
	const char* guidance = "GUIDANCE";
	engine->drawForegroundString(1027, 410, guidance, fontColor, engine->getFont("resources/Cute Rabbit.ttf", 48));
	
	// 64 36
	const char* exit = "EXIT";
	engine->drawForegroundString(1068, 500, exit, fontColor, engine->getFont("resources/Cute Rabbit.ttf", 48));

}

void InitialState::virtMouseDown(int iButton, int iX, int iY)
{
	// Handle left mouse click
	if (iButton == SDL_BUTTON_LEFT)
	{
		if (237 <= iY && iY <= 273 && 996 <= iX && iX <= 1204)
		{
			engine->setState(new GameState(engine));
			engine->virtSetupBackgroundBuffer();
			delete this;
		}
		else if (327 <= iY && iY <= 363 && 1015 <= iX && iX <= 1185)
		{
			engine->setState(new GameState(engine, true));
			engine->virtSetupBackgroundBuffer();
			delete this;
		}
		else if (417 <= iY && iY <= 453 && 1027 <= iX && iX <= 1173)
		{
			engine->setState(new GuidanceState(engine));
			delete this;
		}
		else if (507 <= iY && iY <= 543 && 1068 <= iX && iX <= 1132)
			engine->setExitWithCode(0);
	}
}