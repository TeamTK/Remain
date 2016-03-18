#ifndef _RECOVERY_ITEM_H_
#define _RECOVERY_ITEM_H_

#include "../../GEKO/GEKO.h"

class RecoveryItem : public Task
{
public:
	RecoveryItem(float x, float y, float z);
	~RecoveryItem();
	void Update();
	void Render();

private:
	void HitPlayer(Result_Sphere data);

private:
	float m_Radius;
	StaticMesh m_Model;
	Vector3D m_Pos;
	RenderTask m_Render;
	Collider m_HitSphere;
};

#endif