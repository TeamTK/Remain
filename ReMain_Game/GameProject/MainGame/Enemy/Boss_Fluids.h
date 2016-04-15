#ifndef BOSS_FLUIDS_H
#define BOSS_FLUIDS_H

#include "../../GEKO/GEKO.h"

class Boss_Fluids : public Task
{
public:
	Boss_Fluids(const Vector3D &start, const Vector3D &dir, float speed, float time);
	~Boss_Fluids();
	void Update();
	void HitPlayer(Result_Capsule& data);
	void HitMap(Result_Porygon& r);
private:
	Collider m_Collider;
	Collider m_ColliderMap;
	Vector3D m_Dir;
	Vector3D m_Pos;
	Vector3D m_Oldpos;
	Billboard m_Fluids;
	float m_Speed;
	float m_Cnt;
	float m_LifeTime;
	float m_Radius;
};

#endif