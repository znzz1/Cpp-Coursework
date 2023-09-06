#include "header.h"
#include <fstream>
#include <vector>
#include "GameEngine.h"
#include "GameRecord.h"

UploadState::UploadState(GameEngine* engine)
	:BaseState(engine), userName()
{}

void UploadState::virtSetupBackgroundBuffer()
{
	engine->getBackgroundSurface()->mySDLLockSurface();
	SimpleImage form = ImageManager::loadImage("images/form.png", true);
	form.renderImage(engine->getBackgroundSurface(), 0, 0, 0, 0, engine->getWindowWidth(), engine->getWindowHeight());
	engine->getBackgroundSurface()->mySDLUnlockSurface();
}

void UploadState::virtDrawStringsUnderneath()
{
	const char* prompt = "Please enter you name:";
	engine->drawForegroundString(420, 200, prompt, 0x000000, engine->getFont("resources/Bullpen3D.ttf", 40));

	if(!userName.empty())
		engine->drawForegroundString(450, 360, userName.c_str(), 0x000000, engine->getFont("resources/Montserrat-Bold.ttf", 60));
}

void UploadState::virtKeyDown(int iKeyCode)
{
	if (((97 <= iKeyCode && iKeyCode <= 122) || (48 <= iKeyCode && iKeyCode <= 57)) && userName.length() < 10)
	{
		userName += char(iKeyCode);
		engine->redrawDisplay();
	}
	else if (iKeyCode == SDLK_BACKSPACE && !userName.empty())
	{
		userName.pop_back();
		engine->redrawDisplay();
	}
	else if (iKeyCode == 13)
	{
		if (userName.empty())
			userName = "unknown";

		int score, time;
		ifstream currentGame("archive/current_game_state.txt");
		currentGame >> score >> time;
		currentGame.close();
		GameRecord record(score, time, userName);

		vector<GameRecord> records;
		ifstream historyRecords("archive/top_five_records.txt");
		string name;
		for(int i = 0; i < 5; i++)
		{
			historyRecords >> score >> time >> name;
			records.push_back(GameRecord(score, time, name));
		}
		historyRecords.close();

		auto index = records.begin();
		bool replace = false;
		for (auto i = records.begin(); i != records.end(); i++)
		{
			if (*i < record)
			{
				index = i;
				replace = true;
				break;
			}
		}

		if (replace)
		{
			ofstream newhistoryRecord;
			newhistoryRecord.open("archive/top_five_records.txt", ios::trunc);

			records.insert(index, record);
			for (int i = 0; i < 5; i++)
				newhistoryRecord << records[i];
			newhistoryRecord.close();
		}

		engine->setState(new HistoryState(engine));
		engine->redrawDisplay();
		engine->virtSetupBackgroundBuffer();
		delete this;
	}
}