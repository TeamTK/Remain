#include "Bullet.h"
#include "..\GameSystem\Effect.h"

Bullet::Bullet(const Vector3D &start, const Vector3D &dir, float speed, float time) :
	m_pos(start),
	m_dir(dir),
	m_speed(speed),
	m_Time(time),
	m_cnt(0.0f), 
	Task("Bullet", 1)
{
	//キャラクタとの当たり判定
	m_Sphere.radius = 0.0f;
	m_Collider.Regist_S_vs_C(&m_pos, &m_Sphere.radius, REGIST_FUNC(Bullet::HitCharacter));
	m_Collider.SetID(eHITID1, eHITID0);

	//マップとの当たり判定
	m_ColliderMap.Regist_L_vs_SMesh(&m_Oldpos, &m_pos, REGIST_FUNC(Bullet::HitMap));
	m_ColliderMap.SetID(eHITID2, eHITID3 | eHITID4 | eHITID5 | eHITID6 | eHITID7);

	m_RenderTask.Regist(3, REGIST_RENDER_FUNC(Bullet::Render));
}

Bullet::~Bullet()
{
	m_Collider.Release();
	m_ColliderMap.Release();
}

void Bullet::Update()
{
	m_Oldpos = m_pos;
	m_pos += m_dir * m_speed;			//移動

	//寿命
	if (m_cnt++ > m_Time) Task::SetKill();
}

void Bullet::Render()
{
	Fiqure::RenderBillboard(m_Oldpos, Vector3D(2.0f, 2.0f, 2.0f), "SelectedCursor");
	Fiqure::RenderBillboard(m_pos, Vector3D(2.0f, 2.0f, 2.0f), "SelectedCursor");
}

void Bullet::HitCharacter(Result_Capsule& r)
{
	Task::SetKill();
}

void Bullet::HitMap(Result_Porygon& r)
{
	//エフェクト
	EffectInfo effectData;
	effectData.imageName = "Blood";
	effectData.num = 60;
	effectData.pos = r.contactPos;
	effectData.scale = Vector3D(1.0f, 1.0f, 1.0f);
	effectData.speed = 0.1f;
	effectData.time = 120;
	EffectGeneration::Add(effectData);

	if (r.targetID & eHITID1)
	{
		std::cout << "木" << "\n";
	}

	if (r.targetID & eHITID2)
	{
		std::cout << "家" << "\n";
	}

	if (r.targetID & eHITID3)
	{
		std::cout << "地面" << "\n";
	}

	if (r.targetID & eHITID4)
	{
		std::cout << "壁" << "\n";
	}

	m_ColliderMap.Sleep();

	Task::SetKill();
}