#include "Bullet.h"
#include "../../GameSystem/Effect/EffectSphere.h"
#include "../../GameSystem/Effect/EffectParabola.h"

Bullet::Bullet(const char *taskName, const Vector3D &start, const Vector3D &dir, float speed, float time, float radius) :
	Task(taskName, 1),
	m_Pos(start),
	m_Dir(dir),
	m_Speed(speed),
	m_Time(time),
	m_Cnt(0.0f), 
	m_Radius(radius)
{
	//敵との当たり判定
	m_Collider.Regist_S_vs_C(&m_Pos, &m_Radius, REGIST_FUNC(Bullet::HitCharacter));
	m_Collider.SetID(eHITID1, eHITID0);
	m_Collider.SetName(taskName);

	//マップとの当たり判定
	m_ColliderMap.Regist_L_vs_SMesh(&m_Oldpos, &m_Pos, REGIST_FUNC(Bullet::HitMap));
	m_ColliderMap.SetID(eHITID2, eHITID3 | eHITID4 | eHITID5 | eHITID6 | eHITID7);
}

Bullet::~Bullet()
{
	m_Collider.Release();
	m_ColliderMap.Release();
}

void Bullet::Update()
{
	m_Oldpos = m_Pos;
	m_Pos += m_Dir * m_Speed;	//移動

	//寿命
	if (m_Cnt++ > m_Time) Task::SetKill();
}

void Bullet::HitCharacter(Result_Capsule& r)
{
	Task::SetKill();
}

void Bullet::HitMap(Result_Porygon& r)
{
	Vector3D contactPos = r.contactPos * r.meshMatrix;

	//エフェクト
	EffectInfo effectData;
	effectData.imageName = "MapHit";
	effectData.num = 15;
	effectData.pos = contactPos;
	effectData.scale = Vector3D(0.1f, 0.1f, 0.1f);
	effectData.speed = 0.1f;
	effectData.time = 60;

	if (r.targetName == "Tree_1")
	{
		std::cout << "木" << "\n";
	}

	if (r.targetName == "Tree_2")
	{
		std::cout << "木2" << "\n";
	}

	if (r.targetName == "Cabin")
	{
		std::cout << "家" << "\n";
	}

	if (r.targetName == "Ground")
	{
		std::cout << "地面" << "\n";
		effectData.imageName = "GunEffect";
	}

	if (r.targetName == "RockWall")
	{
		std::cout << "壁" << "\n";
		effectData.imageName = "GunEffect";
	}

	new EffectParabola(effectData, "Map", (m_Oldpos - contactPos).GetNormalize());

	m_ColliderMap.Sleep();
	m_Collider.Sleep();

	Task::SetKill();
}