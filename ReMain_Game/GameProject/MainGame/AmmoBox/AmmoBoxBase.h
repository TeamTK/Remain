#ifndef AMMO_BOX_H
#define AMMO_BOX_H

#include "../../GEKO/GEKO.h"

#define BOX_RADIUS 0.7f

class AmmoBoxBase : public Task
{
public:
	AmmoBoxBase();
	AmmoBoxBase(const char *taskName, unsigned int priority);
	virtual ~AmmoBoxBase();
	void Update();
	void HitPlayer(Result_Sphere& r);

protected:
	int m_AddedNum;	//’e–ò‚ª•â[‚³‚ê‚éŒÂ”
	float m_Radius;	//” ‚Ì”¼Œa
	StaticMesh m_Box;
	Billboard m_Billboard;
	Vector3D m_Pos;
	Collider m_Collider;

};
#endif