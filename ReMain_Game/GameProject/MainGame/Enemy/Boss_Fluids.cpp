#include "Boss_Fluids.h"


Boss_Fluids::Boss_Fluids(const Vector3D &start, const Vector3D &dir, float speed, float time) :
	Task("Boss_Fluids", 1),
	m_Pos(start),
	m_Dir(dir),
	m_Speed(speed),
	m_Cnt(0),
	m_LifeTime(time)
{
	m_RenderTask.Regist(1, REGIST_RENDER_FUNC(Boss_Fluids::Render));

	m_Model.SetAsset("Fluids");
	m_Model.SetTranselate(start);
}

Boss_Fluids::~Boss_Fluids()
{

}

void Boss_Fluids::Update()
{
	m_Cnt += GEKO::GetOneFps();
	if (m_LifeTime < m_Cnt)	Task::SetKill();

	m_Pos += m_Dir * m_Speed;

	m_Model.SetTranselate(m_Pos);
}

void Boss_Fluids::Render()
{
	m_Model.Render();
}
