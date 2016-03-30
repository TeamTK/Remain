#include "AmmoBoxBase.h"

AmmoBoxBase::AmmoBoxBase(const char *taskName, unsigned int priority) :
	Task(taskName, priority), m_Radius(BOX_RADIUS)
{
	m_Box.SetRenderingRegister(true, 1, 0);
	m_Billboard.SetRenderingRegister(true, 1, 0);
}

AmmoBoxBase::AmmoBoxBase()
{

}

AmmoBoxBase::~AmmoBoxBase()
{

}

void AmmoBoxBase::Update()
{

}

void AmmoBoxBase::HitPlayer(Result_Sphere& r)
{
	Task::SetKill();
}