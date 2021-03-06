#include "Character.h"
#include "Gravity.h"

Character::Character(float hp, const char *taskName, unsigned int priority) :
	Task(taskName, priority),
	m_Hp(hp),
	m_IsGravity(false),
	m_BodyRadius(1.0f)
{
	//マップとの判定用
	m_ColliderMap.Regist_S_vs_SMesh(&m_SphereMap.pos, &m_SphereMap.radius, REGIST_FUNC(Character::HitMap));
	m_ColliderMap.SetID(eHITID1 | eHITID2, eHITID0);

	//キャラ同士の体の当たり判定
	m_ColliderBody.Regist_S_vs_S(&m_pos, &m_BodyRadius, REGIST_FUNC(Character::PushBody));
	m_ColliderBody.SetID(eHITID0, eHITID0);

	m_Model.SetRenderingRegister(true, 1, 0);

	m_pGravity = new Gravity(&m_pos, &m_IsGravity);
}

Character::~Character()
{
	m_pGravity->SetKill();
	m_IsGravity = true;
	m_ColliderMap.Release();
	m_ColliderBody.Release();
}

void Character::Update()
{
	m_SphereMap.pos = m_pos + Vector3D(0, m_SphereMap.radius, 0);
	m_Model.SetTranselate(m_pos);
	m_Model.SetRotationRadian(m_rot.x, m_rot.y, m_rot.z);
}

void Character::HitMap(Result_Porygon_Group_Sphere& r)
{
	Vector3D v(0.0f, 0.0f, 0.0f);
	Vector3D normal(0.0f, 0.0f, 0.0f);

	//最大値を取得
	for (int i = 0; i < r.hitNum; i++)
	{
		Result_Porygon_Sphere *poly = &r.pArray[i];
		normal = Vector3D::Matrix3x3(poly->normal, r.meshMatrix);
		Vector3D nv = normal * (m_SphereMap.radius - poly->dist);
		if (fabsf(v.x) < fabsf(nv.x)) v.x = nv.x;
		if (fabsf(v.y) < fabsf(nv.y)) v.y = nv.y;
		if (fabsf(v.z) < fabsf(nv.z)) v.z = nv.z;
	}
	m_pos += v;
}

void Character::PushBody(Result_Sphere &data)
{
	Vector3D vec = m_pos - data.position;
	float l = (m_BodyRadius + data.radius) - vec.Length();

	m_pos += vec.GetNormalize() * (l * 0.1f);
}