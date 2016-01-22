#ifndef AMMO_BOX_H
#define AMMO_BOX_H

#include "../../GEKO/GEKO.h"
#include "../../GEKO/Collider/Collider.h"

#define BOX_RADIUS 0.7f

class AmmoBoxBase : public Task
{
public:
	AmmoBoxBase();
	AmmoBoxBase(Vector3D pos, Vector3D rot, const char *taskName, unsigned int priority);
	virtual ~AmmoBoxBase();
	void Update();
	void Render();
	void HitPlayer(Result_Sphere& r);

protected:
	int m_AddedNum;	//’e–ò‚ª•â[‚³‚ê‚éŒÂ”
	float m_Radius;	//” ‚Ì”¼Œa
	StaticMesh m_Box;
	Vector3D m_Pos;
	RenderTask m_RenderTask;
	Collider m_Collider;

};
#endif