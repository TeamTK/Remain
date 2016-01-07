#include "Character.h"

Character::Character(float hp, const char *taskName, unsigned int priority) :
	Task(taskName, priority),
	m_Hp(hp)
{
	//マップとの判定用
	m_ColliderMap.Regist_S_vs_SMesh(&m_SphereMap.pos, &m_SphereMap.radius, REGIST_FUNC(Character::HitMap));
	m_ColliderMap.SetID(eHITID1 | eHITID2, eHITID0);

	m_BodyRadius = 1.0f;
	m_ColliderBody.Regist_S_vs_S(&m_pos, &m_BodyRadius, REGIST_FUNC(Character::PushBody));
	m_ColliderBody.SetID(eHITID0, eHITID0);

	m_RenderTask.Regist(0, REGIST_RENDER_FUNC(Character::Render));
}

Character::~Character()
{

}

void Character::Update()
{
	m_SphereMap.pos = m_pos + Vector3D(0, m_SphereMap.radius, 0);
}

void Character::Render()
{
	m_Model.SetTranselate(m_pos);
	m_Model.SetRotationRadian(m_rot.x, m_rot.y, m_rot.z);
	m_Model.Render();
	m_pos.y -= 0.01f;
}

void Character::HitMap(Result_Porygon_Group& r)
{
	Vector3D v(0, 0, 0);
	for (int i = 0; i < r.hitNum; i++)
	{
		Result_Porygon_Sphere *poly = &r.pArray[i];
		Vector3D nv = poly->normal * (m_SphereMap.radius - poly->dist);
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