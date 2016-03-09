#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "../GEKO/GEKO.h"
#include "../GameSystem/FiveSenses/Auditory.h"
#include "../GameSystem/FiveSenses/SightSystem.h"

class Gravity;

class Character : public Task
{
public:
	Character() {}
	Character(float hp, const char *taskName, unsigned int priority);
	virtual ~Character();
	virtual void Update();
	void Render();
	void HitMap(Result_Porygon_Group_Sphere& r);
	void PushBody(Result_Sphere &data);

protected:
	bool m_IsGravity;
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
	Gravity *m_pGravity;
};

#endif