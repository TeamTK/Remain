#pragma once

#include "../GEKO/GEKO.h"
#include "CBullet.h"

class Character : public Task
{
public:
	Character() {}
	Character(float hp, char *taskName, unsigned int priority);
	virtual ~Character();
	virtual void Update();
	virtual void Render();
	virtual void HitBullet();
	void Capsule_vs_LineSegmentCallback(Result_Sphere& r);
	void Sphere_vs_MeshCallback(Result_Porygon_Group& r);
	void PushBody(Result_Sphere &data);

protected:
	float m_Hp;
	Vector3D m_pos;
	Vector3D m_rot;
	DynamicMesh m_Model;

	//�}�b�v�Ƃ̓����蔻��
	Collider m_ColliderMap;
	SphereInfo m_SphereMap;

	//�L�����N�^���m�̑̂̓����蔻��
	Collider m_ColliderBody;
	float m_BodyRadius;

	RenderTask m_RenderTask;
};