#pragma once
#include "CCharacter.h"
#include "..\GameSystem\SightSystem.h"

class CEnemy : public CCharacter 
{
public:
	enum 
	{
		eAnimationWalk,
		eAnimationAttack,
		eAnimationDie,
		eAnimationHitDamage,
		eAnimationIdle,
		eAnimationTrot,

	};
	enum 
	{
		eState_Attack,
		eState_Idle,
		eState_Chase,
		eState_HitDamage,
		eState_Die
	};

	CEnemy(int type,Vector3D pos);
	void Attack();
	void Idle();
	void Chase();
	void HitDamage();
	void Die();
	void Update();
	void HitBullet();

private:
	bool m_isChase;
	int m_FlinchNum;
	int m_state;
	EnemySight m_Sight;
	SightData m_SightData;
	Vector3D m_SightVec;
	Vector3D m_Distance;
	const Vector3D *m_pPlayerPos;

private:
	void HitSight(const Vector3D *pPos);
};

class CEnemyManager 
{
public:
	static CEnemy *Add(int type,Vector3D pos);
};