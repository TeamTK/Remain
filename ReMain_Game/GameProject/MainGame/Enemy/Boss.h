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
	unsigned int m_AnimType; //アニメーションの種類
	float m_Hp;			//体力
	float m_AnimSpeed; //アニメーションの速度
	float m_AnimEndTime; //アニメーションの終わり時間
	float m_OneFlameTime;	//1フレーム時間

};

#endif