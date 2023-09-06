#include "header.h"
#include "GameEngine.h"
#include "GameRecord.h"
#include "ExampleDragableObjects.h"

HistoryState::HistoryState(GameEngine* engine)
	:BaseState(engine), records(), partialShift(0, 0, 0.5)
{
	int score, time;
	string name;

	ifstream historyRecords("archive/top_five_records.txt");

	for (int i = 0; i < 5; i++)
	{
		historyRecords >> score >> time >> name;
		records.push_back(GameRecord(score, time, name));
	}

	historyRecords.close();

	virtInitialiseObjects();
}

HistoryState::~HistoryState()
{
	engine->destroyOldObjects(true);
}

void HistoryState::drawObjects()
{
	engine->BaseEngine::drawAllObjects();
}

void HistoryState::virtMainLoopDoBeforeUpdate()
{
	int iOffset = engine->getModifiedTime() / 10;
	partialShift.setYShift((double)iOffset);
	engine->redrawDisplay();
}

int HistoryState::virtInitialiseObjects()
{
	engine->drawableObjectsChanged();
	engine->destroyOldObjects(true);
	engine->createObjectArray(1);

	MappableImageObject* ob = new MappableImageObject(&partialShift, 100, 700, engine, "images/cardback.png", false);
	ob->setTransparencyColour(ob->getPixelColour(0, 0));

	engine->storeObjectInArray(0, ob);

	return 0;
}

void HistoryState::virtSetupBackgroundBuffer()
{
	SimpleImage rankingList = ImageManager::loadImage("images/ranking.png", true);
	engine->getBackgroundSurface()->mySDLLockSurface();
	rankingList.renderImageWithMask(engine->getBackgroundSurface(), 0, 0, 0, 0, engine->getWindowWidth(), engine->getWindowHeight());

	const char* prompt1 = "TOP 5 HISTORY RECORDS!";
	engine->drawBackgroundString(220, 60, prompt1, 0xcccccc, engine->getFont("resources/Bullpen3D.ttf", 60));

	const char* prompt2 = "RANKING       NAME            SCORE        TIME";
	engine->drawBackgroundString(200, 180, prompt2, 0xaaaaaa, engine->getFont("resources/Bullpen3D.ttf", 40));

	const char* r1 = "1";
	const char* n1 = (records[0].name).c_str();
	string s1 = to_string(records[0].score);
	string t1 = to_string(records[0].time);
	engine->drawBackgroundString(280, 240, r1, 0xffffff, engine->getFont("resources/Cornerstone Regular.ttf", 40));
	engine->drawBackgroundString(450, 240, n1, 0xffffff, engine->getFont("resources/Cornerstone Regular.ttf", 40));
	engine->drawBackgroundString(790, 240, s1.c_str(), 0xffffff, engine->getFont("resources/Cornerstone Regular.ttf", 40));
	engine->drawBackgroundString(955, 240, t1.c_str(), 0xffffff, engine->getFont("resources/Cornerstone Regular.ttf", 40));

	const char* r2 = "2";
	const char* n2 = (records[1].name).c_str();
	string s2 = to_string(records[1].score);
	string t2 = to_string(records[1].time);
	engine->drawBackgroundString(280, 290, r2, 0xffffff, engine->getFont("resources/Cornerstone Regular.ttf", 40));
	engine->drawBackgroundString(450, 290, n2, 0xffffff, engine->getFont("resources/Cornerstone Regular.ttf", 40));
	engine->drawBackgroundString(790, 290, s2.c_str(), 0xffffff, engine->getFont("resources/Cornerstone Regular.ttf", 40));
	engine->drawBackgroundString(955, 290, t2.c_str(), 0xffffff, engine->getFont("resources/Cornerstone Regular.ttf", 40));

	const char* r3 = "3";
	const char* n3 = (records[2].name).c_str();
	string s3 = to_string(records[2].score);
	string t3 = to_string(records[2].time);
	engine->drawBackgroundString(280, 340, r3, 0xffffff, engine->getFont("resources/Cornerstone Regular.ttf", 40));
	engine->drawBackgroundString(450, 340, n3, 0xffffff, engine->getFont("resources/Cornerstone Regular.ttf", 40));
	engine->drawBackgroundString(790, 340, s3.c_str(), 0xffffff, engine->getFont("resources/Cornerstone Regular.ttf", 40));
	engine->drawBackgroundString(955, 340, t3.c_str(), 0xffffff, engine->getFont("resources/Cornerstone Regular.ttf", 40));

	const char* r4 = "4";
	const char* n4 = (records[3].name).c_str();
	string s4 = to_string(records[3].score);
	string t4 = to_string(records[3].time);
	engine->drawBackgroundString(280, 390, r4, 0xffffff, engine->getFont("resources/Cornerstone Regular.ttf", 40));
	engine->drawBackgroundString(450, 390, n4, 0xffffff, engine->getFont("resources/Cornerstone Regular.ttf", 40));
	engine->drawBackgroundString(790, 390, s4.c_str(), 0xffffff, engine->getFont("resources/Cornerstone Regular.ttf", 40));
	engine->drawBackgroundString(955, 390, t4.c_str(), 0xffffff, engine->getFont("resources/Cornerstone Regular.ttf", 40));

	const char* r5 = "5";
	const char* n5 = (records[4].name).c_str();
	string s5 = to_string(records[4].score);
	string t5 = to_string(records[4].time);
	engine->drawBackgroundString(280, 440, r5, 0xffffff, engine->getFont("resources/Cornerstone Regular.ttf", 40));
	engine->drawBackgroundString(450, 440, n5, 0xffffff, engine->getFont("resources/Cornerstone Regular.ttf", 40));
	engine->drawBackgroundString(790, 440, s5.c_str(), 0xffffff, engine->getFont("resources/Cornerstone Regular.ttf", 40));
	engine->drawBackgroundString(955, 440, t5.c_str(), 0xffffff, engine->getFont("resources/Cornerstone Regular.ttf", 40));

	const char* back = "Back To Main";
	engine->drawBackgroundString(920, 680, back, 0xaaaaaa, engine->getFont("resources/Cute Rabbit.ttf", 52));
	engine->getBackgroundSurface()->mySDLUnlockSurface();
}

void HistoryState::virtMouseDown(int iButton, int iX, int iY)
{
	// Handle left mouse click
	if (iButton == SDL_BUTTON_LEFT)
	{
		if (680 <= iY && iY <= 728 && 920 <= iX && iX <= 1140)
		{
			engine->setState(new InitialState(engine));
			engine->redrawDisplay();
			engine->virtCreateSurfaces();
			engine->virtSetupBackgroundBuffer();
			delete this;
		}
	}
}
