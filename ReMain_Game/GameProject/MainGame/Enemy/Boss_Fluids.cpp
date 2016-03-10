#include "Boss_Fluids.h"
#include "../../GameSystem/Effect/EffectParabola.h"


Boss_Fluids::Boss_Fluids(const Vector3D &start, const Vector3D &dir, float speed, float lifeTime) :
	Task("Boss_Fluids", 1),
	m_LifeTime(lifeTime),
	m_Oldpos(start),
	m_Speed(speed),
	m_Pos(start),
	m_Dir(dir),
	m_Radius(0.1f),
	m_Cnt(0)
{
	//プレイヤーとの当たり判定
	m_Collider.Regist_C_vs_C(&m_Pos, &m_Pos, &m_Radius, REGIST_FUNC(Boss_Fluids::HitPlayer));
	m_Collider.SetID(eHITID2, eHITID0);
	//マップとの当たり判定
	m_ColliderMap.Regist_L_vs_SMesh(&m_Oldpos, &m_Pos, REGIST_FUNC(Boss_Fluids::HitMap));
	m_ColliderMap.SetID(eHITID8, eHITID4 | eHITID6 | eHITID7);
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

void Boss_Fluids::HitMap(Result_Porygon& r)
{
	Vector3D contactPos = r.contactPos * r.meshMatrix;
	
	//エフェクト
	EffectInfo effectData;
	effectData.imageName = "Fluids_Par";
	effectData.num = 10;
	effectData.pos = contactPos;
	effectData.size = 0.1f;
	effectData.speed = 0.1f;
	effectData.time = 60;
	
	new EffectParabola(effectData, "HitMap", (m_Oldpos - contactPos).GetNormalize());

	m_ColliderMap.Sleep();
	m_Collider.Sleep();

	Task::SetKill();
}