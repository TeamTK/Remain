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
	void HitBullet(Result_Sphere& r);	//銃弾との当たり判定
	void HitAttack(Result_Capsule &hitData);	//プレイヤーへの攻撃の当たり判定
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
	Collider *m_pHitAttackBody;	//銃弾との当たり判定
	Collider *m_pHitAttack;		//プレイヤーへの攻撃の当たり判定
	FunctionTask m_FuncTask;	//AI管理
	Vector3D m_pos;
	Vector3D* m_pPlayerPos;
	std::vector<BoneCalpule> m_BoneCapsule; //ボーン情報（カプセル）
	std::vector<float> m_DamageMagnification; //ダメージ倍率
	unsigned int m_AnimType;	//アニメーションの種類
	int m_FlinchNum; //耐久値
	float m_FlinchCnt; //耐久蓄積値
	float m_Timer;
	float m_Hp;			//体力
	float m_AnimSpeed;	//アニメーションの速度
	float m_AnimEndTime;	//アニメーションの終わり時間
	float m_OneFlameTime;	//1フレーム時間
	float m_Length;
	bool m_isDefence;

};

#endif