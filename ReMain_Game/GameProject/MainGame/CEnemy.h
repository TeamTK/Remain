#pragma once
#include "CCharacter.h"
#include "..\GameSystem\SightSystem.h"

struct BoneCalpule
{
	float radius;
	int start;
	int end;
	BoneCalpule(float r, int s, int e)
	{
		radius = r;
		start = s;
		end = e;
	}
};

class CEnemy : public Character
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

	CEnemy(Vector3D pos, Vector3D rot);
	~CEnemy();
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

	//キャラクタ同士の攻撃された時の当たり判定
	Collider *m_pCollider;
	CapsuleInfo *m_pCapsule;
	std::vector<BoneCalpule> BoneCapsule;

private:
	void HitSight(const Vector3D *pPos);
};