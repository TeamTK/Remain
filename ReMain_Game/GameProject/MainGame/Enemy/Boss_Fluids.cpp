#include "Boss_Fluids.h"
#include "../../GameSystem/Effect/EffectParabola.h"


Boss_Fluids::Boss_Fluids(const Vector3D &start, const Vector3D &dir, float speed, float lifeTime) :
	Task("Boss_Fluids", 1),
	m_Pos(start),
	m_Dir(dir),
	m_Speed(speed),
	m_Cnt(0),
	m_LifeTime(lifeTime),
	m_Radius(0.1f)
{
	//プレイヤーとの当たり判定
	m_Collider.Regist_C_vs_C(&m_Pos, &m_Pos, &m_Radius, REGIST_FUNC(Boss_Fluids::HitPlayer));
	m_Collider.SetID(eHITID2, eHITID0);
}

Boss_Fluids::~Boss_Fluids()
{
	m_Collider.Release();
	TaskManager::Kill("Fluids");
}

void Boss_Fluids::Update()
{
	m_Cnt += GEKO::GetOneFps();

	//エフェクト
	EffectInfo effectData;
	effectData.imageName = "Fluids_Par";
	effectData.num = 12;
	effectData.pos = m_Pos;
	effectData.size = 0.2f;
	effectData.speed = 0.1f;
	effectData.time = 40;
	new EffectParabola(effectData, "Fluids", m_Dir);

	//パーティクル削除
	if (m_Cnt < 0.5f)	TaskManager::Kill("Fluids");


	if (m_LifeTime < m_Cnt)
	{
		Task::SetKill();
	}

	m_Pos += m_Dir * m_Speed;
}

void Boss_Fluids::HitPlayer(Result_Capsule& data)
{
	Task::SetKill();
}
