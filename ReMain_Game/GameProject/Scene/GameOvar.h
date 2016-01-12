#ifndef _GAMEOVAR_H_
#define _GAMEOVAR_H_

#include "..\GEKO\Task\Task.h"

class GameOvar : public Task
{
public:
	GameOvar();
	~GameOvar();
	void Update();
	void Render();

private:

};

#endif