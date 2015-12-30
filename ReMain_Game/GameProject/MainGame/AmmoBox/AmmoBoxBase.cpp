#include "AmmoBoxBase.h"

AmmoBoxBase::AmmoBoxBase(Vector3D pos, Vector3D rot, const char *taskName, unsigned int priority) :
	Task(taskName, priority), m_Radius(0.3f), m_Pos(pos)
{

	m_RenderTask.Regist(0, REGIST_RENDER_FUNC(AmmoBoxBase::Render));
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

void AmmoBoxBase::Render()
{
	m_Box.Render();
}

void AmmoBoxBase::HitPlayer(Result_Sphere& r)
{
	Task::SetKill();
}