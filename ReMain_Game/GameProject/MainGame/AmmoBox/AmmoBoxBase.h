#ifndef AMMO_BOX_H
#define AMMO_BOX_H

#include "../../GEKO/GEKO.h"
#include "../../GEKO/Collider/Collider.h"

class AmmoBoxBase : public Task
{
public:
	AmmoBoxBase();
	AmmoBoxBase(Vector3D pos, Vector3D rot);
	virtual ~AmmoBoxBase();
	void Update();
	void Render();
	void HitCharacter(Result_Sphere& r);

private:
	float m_Radius;
	StaticMesh m_Box;
	Vector3D m_Pos;
	RenderTask m_RenderTask;
	Collider m_Collider;

};
#endif