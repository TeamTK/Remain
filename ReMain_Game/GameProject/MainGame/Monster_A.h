#ifndef _MONSTER_A_
#define _MONSTER_A_

#include "Enemy.h"

class Monster_A : public Enemy
{
public:
	Monster_A(Vector3D pos, Vector3D rot);
	~Monster_A();
	void Update();

private:

};

#endif