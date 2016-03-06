#ifndef BOSS_H
#define BOSS_H

#include "../../GEKO/GEKO.h"
#include "../Enemy/Enemy.h"

struct BossState
{
	float hp;
	int flinch;
	Vector3D spawnPos;
	Vector3D spawnRot;
};

class Boss : public Task
{
	enum
	{
		eAnimationAttack,
		eAnimationHitDamage,
		eAnimationDefence,
		eAnimationDie,
		eAnimationIdle,
	};

public:
	Boss(BossState &bossState);
	~Boss();
	void HitBullet(Result_Sphere& r);	//�e�e�Ƃ̓����蔻��
	void HitAttack(Result_Capsule &hitData);	//�v���C���[�ւ̍U���̓����蔻��
	void Update();
	void Render();

	void Attack();
	void Defence();
	void Idle();
	void HitDamage();
	void Die();

private:
	DynamicMesh m_Model;
	RenderTask m_RenderTask;
	CapsuleInfo *m_pCapsule;
	Collider *m_pHitAttackBody;	//�e�e�Ƃ̓����蔻��
	Collider *m_pHitAttack;		//�v���C���[�ւ̍U���̓����蔻��
	FunctionTask m_FuncTask;	//AI�Ǘ�
	Vector3D m_pos;
	Vector3D* m_pPlayerPos;
	std::vector<BoneCalpule> m_BoneCapsule; //�{�[�����i�J�v�Z���j
	std::vector<float> m_DamageMagnification; //�_���[�W�{��
	unsigned int m_AnimType;	//�A�j���[�V�����̎��
	int m_FlinchNum; //�ϋv�l
	float m_FlinchCnt; //�ϋv�~�ϒl
	float m_Timer;
	float m_Hp;			//�̗�
	float m_AnimSpeed;	//�A�j���[�V�����̑��x
	float m_AnimEndTime;	//�A�j���[�V�����̏I��莞��
	float m_OneFlameTime;	//1�t���[������
	float m_Length;
	bool m_isDefence;

};

#endif