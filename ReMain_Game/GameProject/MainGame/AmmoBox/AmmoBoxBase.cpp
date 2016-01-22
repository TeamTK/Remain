#include "AmmoBoxBase.h"

AmmoBoxBase::AmmoBoxBase(Vector3D pos, Vector3D rot, const char *taskName, unsigned int priority) :
	Task(taskName, priority), m_Radius(BOX_RADIUS), m_Pos(pos)
{

	m_RenderTask.Regist(1, REGIST_RENDER_FUNC(AmmoBoxBase::Render));
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
	if (Input::KeyQ.Clicked() || Input::XInputPad1.AClicked())
		Task::SetKill();
}