#ifndef BOSS_H
#define BOSS_H

#include "../../GEKO/GEKO.h"

struct BossState
{
	float hp;
	Vector3D spawnPos;
	Vector3D spawnRot;
};

class Boss : public Task
{
	enum
	{
		eAnimationAttack,
		eAnimationHitDamage,
		eAnimationIdle,
		//eAnimationDie,
	};

public:
	Boss(BossState &bossState);
	~Boss();
	void Attack();
	void Idle();
	void HitDamage();
	void Die();
	void Update();
	void Render();

private:
	DynamicMesh m_Model;
	RenderTask m_RenderTask;
	unsigned int m_AnimType; //�A�j���[�V�����̎��
	float m_Hp;			//�̗�
	float m_AnimSpeed; //�A�j���[�V�����̑��x
	float m_AnimEndTime; //�A�j���[�V�����̏I��莞��
	float m_OneFlameTime;	//1�t���[������

};

#endif