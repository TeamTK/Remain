#pragma once
#include "CCharacter.h"

class CEnemy : public CCharacter {
public:
	enum {
		eAnimationWalk,
		eAnimationAttack,
		eAnimationDie,
		eAnimationHit,
		eAnimationIdle,
		eAnimationTrot,

	};
	enum {
		eState_Idle,
		eState_Hit,
		eState_Die
	};
	int m_state;
	int m_cnt;
	CEnemy(int type,Vector3D pos);
	void Hit();
	void Idle();
	void Die();
	void Update();
	void HitBullet();
};

class CEnemyManager {
public:
	static CEnemy *Add(int type,Vector3D pos);
};