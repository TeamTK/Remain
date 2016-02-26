#ifndef _MONSTER_B_
#define _MONSTER_B_

#include "Enemy.h"

class Monster_B : public Enemy
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
	Monster_B(EnemyState &enemyState);
	~Monster_B();
	void Attack();
	void Idle();
	void Chase();
	void HitDamage();
	void Die();
	void Wandering();
	void Update();

};

#endif