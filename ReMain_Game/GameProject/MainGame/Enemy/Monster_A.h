#ifndef _MONSTER_A_
#define _MONSTER_A_

#include "Enemy.h"

class Monster_A : public Enemy
{
	enum
	{
		eAnimationWalk,
		eAnimationAttack,
		eAnimationDie,
		eAnimationHitDamage,
		eAnimationIdle,
		eAnimationTrot,
	};

public:
	Monster_A(EnemyState &enemyState);
	~Monster_A();
	void Attack();
	void Idle();
	void Chase();
	void HitDamage();
	void Die();
	void Update();

private:

};

#endif