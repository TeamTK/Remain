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
		eAnimDie_Front,
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
	void Update();

};

#endif