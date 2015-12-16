#pragma once
#include "CCharacter.h"
#include "..\GameSystem\SightSystem.h"

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
		eState_Chase,
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

private:
	EnemySight m_Sight;
	SightData m_SightData;
	Vector3D m_SightVec;
	const Vector3D *m_pPlayerPos;

private:
	void HitSight(const Vector3D *pPos);
};

class CEnemyManager {
public:
	static CEnemy *Add(int type,Vector3D pos);
};