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
	int m_AddedNum;	//�e�򂪕�[������
	float m_Radius;	//���̔��a
	StaticMesh m_Box;
	Billboard m_Billboard;
	Vector3D m_Pos;
	Collider m_Collider;

};
#endif