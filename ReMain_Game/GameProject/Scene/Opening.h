#ifndef _OPENING_H_
#define _OPENING_H_

#include "..\GEKO\Task\Task.h"

class Opening : public Task
{
public:
	Opening();
	~Opening();
	void Update();
	void Render();

private:
};

#endif