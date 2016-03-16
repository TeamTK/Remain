#ifndef _MAINGAME_H_
#define _MAINGAME_H_

#include "../Chapter/Chapter.h"

class MainGame : public Task
{
public:
	MainGame(ChapterType type);
	~MainGame();
	void Update();

private:

};

#endif