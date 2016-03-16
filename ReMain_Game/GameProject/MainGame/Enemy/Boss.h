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
		eAnimationAttack1,
		eAnimationAttack2,
		eAnimationHitDamage,
		eAnimationDefence,
		eAnimationDie,
		eAnimationIdle,
		eAnimLongAttack,
	};

public:
	Boss(BossState &bossState);
	~Boss();
	void HitBullet(Result_Sphere& r);	//�e�e�Ƃ̓����蔻��
	void HitAttack(Result_Capsule &hitData);	//�v���C���[�ւ̍U���̓����蔻��
	void Update();
	void Render();

	void Attack_A();
	void Attack_B();
	void LongAttack();
	void Defence();
	void Idle();
	void HitDamage();
	void Die();

private:
	DynamicMesh m_Model;
	RenderTask m_RenderTask;
	Timer m_Timer;
	Timer m_NoActionTime;
	CapsuleInfo *m_pCapsule;
	Collider *m_pHitAttackBody;	//�e�e�Ƃ̓����蔻��
	Collider *m_pHitAttack;		//�v���C���[�ւ̍U���̓����蔻��
	FunctionTask m_FuncTask;	//AI�Ǘ�
	Vector3D m_Pos;
	Vector3D m_Rot;
	Vector3D* m_pPlayerPos;
	std::vector<BoneCalpule> m_BoneCapsule; //�{�[�����i�J�v�Z���j
	std::vector<float> m_DamageMagnification; //�_���[�W�{��
	unsigned int m_AnimType;	//�A�j���[�V�����̎��
	int m_FlinchNum; //�ϋv�l
	float m_FlinchCnt; //�ϋv�~�ϒl
	float m_Hp;			//�̗�
	float m_MaxHp;		//�ő�̗�
	float m_AnimSpeed;	//�A�j���[�V�����̑��x
	float m_AnimEndTime;	//�A�j���[�V�����̏I��莞��
	float m_OneFlameTime;	//1�t���[������
	float m_PlayTime;		//�A�j���[�V�����Đ��t���[��
	float m_Length;			//�v���C���[�Ƃ̋���
	bool m_isDefence;		//�h�䂵�Ă��邩
	bool m_isAlive;			//�����Ă��邩

};

#endif