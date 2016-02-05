#ifndef _EVENT_SCENE_BASE_H_
#define _EVENT_SCENE_BASE_H_

#include "../../GEKO/GEKO.h"

class EventSceneBase : public Task
{
public:
	EventSceneBase(const char* name, unsigned int priority);
	virtual ~EventSceneBase();
	virtual void Update() = 0;

private:

};

#endif