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

	Enemy(Vector3D pos, Vector3D rot, const char* name);
	virtual ~Enemy();
	virtual void Update();
	void HitBullet(Result_Sphere& r);
	void HitAttack(Result_Capsule &hitData);

protected:
	void Attack();
	void Idle();
	void Chase();
	void HitDamage();
	void Die();

protected:
	int m_FlinchNum;
	int m_state;

	//敵の視界
	EnemySight m_Sight;
	SightData m_SightData;
	Vector3D m_SightPos;
	Vector3D m_SightVec;

	Vector3D m_Distance;
	const Vector3D *m_pPlayerPos;

	//プレイヤーからの攻撃された時の当たり判定
	Collider *m_pCollider;
	CapsuleInfo *m_pCapsule;
	std::vector<BoneCalpule> m_BoneCapsule;

	//プレイヤーへの攻撃の当たり判定
	Collider *m_pHitAttack; 

private:
	void HitSight(const Vector3D *pPos);
};

#endif