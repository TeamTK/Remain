#ifndef BOSS_FLUIDS_H
#define BOSS_FLUIDS_H

#include "../../GEKO/GEKO.h"
#include "../../MainGame/Player/Player.h"

class Boss_Fluids : public Task
{
public:
	Boss_Fluids(const Vector3D &start, const Vector3D &dir, float speed, float time);
	~Boss_Fluids();
	void Update();
	void Render();
private:
	StaticMesh m_Model;
	RenderTask m_RenderTask;
	Vector3D m_Dir;
	Vector3D m_Pos;
	float m_Speed;
	float m_Cnt;
	float m_LifeTime;
};

#endif