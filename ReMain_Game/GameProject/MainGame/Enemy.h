#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "Character.h"
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

class Enemy : public Character
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

	Enemy(Vector3D pos, Vector3D rot);
	~Enemy();
	void Attack();
	void Idle();
	void Chase();
	void HitDamage();
	void Die();
	void Update();
	void HitBullet(Result_Sphere& r);

private:
	bool m_isChase;
	int m_FlinchNum;
	int m_state;

	//“G‚Ì‹ŠE
	EnemySight m_Sight;
	SightData m_SightData;
	Vector3D m_SightVec;

	Vector3D m_Distance;
	const Vector3D *m_pPlayerPos;

	//ƒLƒƒƒ‰ƒNƒ^“¯m‚ÌUŒ‚‚³‚ê‚½‚Ì“–‚½‚è”»’è
	Collider *m_pCollider;
	CapsuleInfo *m_pCapsule;
	std::vector<BoneCalpule> BoneCapsule;

private:
	void HitSight(const Vector3D *pPos);
};

#endif