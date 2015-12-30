#include "Bullet.h"
#include "..\GameSystem\Effect.h"

Bullet::Bullet(const Vector3D &start, const Vector3D &dir, float speed, float time, float radius) :
	Task("Bullet", 1),
	m_Pos(start),
	m_Dir(dir),
	m_Speed(speed),
	m_Time(time),
	m_Cnt(0.0f), 
	m_Radius(radius)
{
	//�L�����N�^�Ƃ̓����蔻��
	m_Collider.Regist_S_vs_C(&m_Pos, &m_Radius, REGIST_FUNC(Bullet::HitCharacter));
	m_Collider.SetID(eHITID1, eHITID0);

	//�}�b�v�Ƃ̓����蔻��
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
	m_Pos += m_Dir * m_Speed;			//�ړ�

	//����
	if (m_Cnt++ > m_Time) Task::SetKill();
}

void Bullet::HitCharacter(Result_Capsule& r)
{
	Task::SetKill();
}

void Bullet::HitMap(Result_Porygon& r)
{
	//�G�t�F�N�g
	EffectInfo effectData;
	effectData.imageName = "Blood";
	effectData.num = 60;
	effectData.pos = r.contactPos;
	effectData.scale = Vector3D(1.0f, 1.0f, 1.0f);
	effectData.speed = 0.1f;
	effectData.time = 120;
	new Effect(effectData, "Map");

	if (r.targetID & eHITID4)
	{
		std::cout << "��" << "\n";
	}

	if (r.targetID & eHITID5)
	{
		std::cout << "��" << "\n";
	}

	if (r.targetID & eHITID6)
	{
		std::cout << "�n��" << "\n";
	}

	if (r.targetID & eHITID7)
	{
		std::cout << "��" << "\n";
	}

	m_ColliderMap.Sleep();
	m_Collider.Sleep();
	//m_Collider.Release();

	Task::SetKill();
}