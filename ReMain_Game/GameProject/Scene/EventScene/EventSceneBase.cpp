#include "EventSceneBase.h"

EventSceneBase::EventSceneBase(const char* name, unsigned int priority) :
	Task(name, priority)
{
}

EventSceneBase::~EventSceneBase()
{
}