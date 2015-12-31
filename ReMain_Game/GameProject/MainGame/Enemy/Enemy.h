#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "..\Character.h"
#include "..\..\GameSystem\SightSystem.h"
#include "..\..\GEKO\Task\FunctionTask.h"

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
		eState_Attack,
		eState_Idle,
		eState_Chase,
		eState_HitDamage,
		eState_Die
	};

	Enemy(Vector3D pos, Vector3D rot, const char* name, int flinchNum);
	virtual ~Enemy();
	virtual void Update();
	void HitBullet(Result_Sphere& r);
	void HitAttack(Result_Capsule &hitData);

protected:
	void Attack(unsigned int animNum, int animEndTime);
	void Idle(unsigned int animNum);
	void Chase(unsigned int animNum);
	void HitDamage(unsigned int animNum, int animEndTime);
	void Die(unsigned int animNum, int animEndTime);

private:
	void HitSight(const Vector3D *pPos);

protected:
	int m_FlinchNum;
	int m_FlinchCnt;

	//�G�̎��E
	EnemySight m_Sight;
	SightData m_SightData;
	Vector3D m_SightPos;
	Vector3D m_SightVec;

	//�ǐ՗p
	Vector3D m_Distance;
	const Vector3D *m_pPlayerPos;

	//�v���C���[����̍U�����ꂽ���̓����蔻��
	Collider *m_pCollider;
	CapsuleInfo *m_pCapsule;
	std::vector<BoneCalpule> m_BoneCapsule;

	Collider *m_pHitAttack; //�v���C���[�ւ̍U���̓����蔻��
	FunctionTask m_FuncTask; //AI�Ǘ�
};

#endif