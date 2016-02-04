#ifndef _GRAVITY_H_
#define _GRAVITY_H_

#include "..\GEKO\Task\Task.h"

class Vector3D;

class Gravity : public Task
{
public:
	Gravity(Vector3D *pPos, bool *pisKill);
	~Gravity();
	void Update();

private:
	Vector3D *m_pPos;
	bool *m_pIsKill;
};

#endif