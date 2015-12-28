#include "AmmoBoxBase.h"
#include "../../GEKO/Figure/Fiqure.h"

AmmoBoxBase::AmmoBoxBase(Vector3D pos, Vector3D rot) :
	Task("AmmoBox", 0), m_Radius(0.3f), m_Pos(0.0f, 0.0f, 0.0f)
{
	m_RenderTask.Regist(0, REGIST_RENDER_FUNC(AmmoBoxBase::Render));
	m_Box.SetAsset("AmmoBox");
	m_Box.SetTranselate(pos);
	m_Box.SetRotationRadian(rot.x, rot.y, rot.z);

	//ÉvÉåÉCÉÑÅ[Ç∆ÇÃìñÇΩÇËîªíË
	m_Collider.Regist_S_vs_S(&m_Pos, &m_Radius, REGIST_FUNC(AmmoBoxBase::HitCharacter));
	m_Collider.SetID(eHITID2, eHITID1);
}

AmmoBoxBase::AmmoBoxBase()
{

}

AmmoBoxBase::~AmmoBoxBase()
{

}

void AmmoBoxBase::Update()
{
	m_Pos = m_Box.GetTranselate();
}

void AmmoBoxBase::Render()
{
	Fiqure::RenderBillboard(m_Pos + Vector3D(0.0f, 2.0f, 0.0f), Vector3D(0.5f, 0.5f, 0.5f), "UI_Shotgun");
	m_Box.Render();
}

void AmmoBoxBase::HitCharacter(Result_Sphere& r)
{
	printf("Getting Ammo Box!\n");
	Task::SetKill();
}