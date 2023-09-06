#include <string>
#include <fstream>
using namespace std;

class GameRecord
{
public:
	int score;
	int time;
	string name;
	
	GameRecord(int score, int time, string name)
		:score(score), time(time), name(name)
	{ }

	friend ostream& operator<<(ostream& out, GameRecord& record)
	{
		out << record.score << " " << record.time << " " << record.name << "\n";
		return out;
	}

	friend bool operator<(GameRecord& A, GameRecord& B)
	{
		if (A.score < B.score)
			return true;
		if (A.score == B.score)
			return !(A.time < B.time);
		return false;
	}

};

