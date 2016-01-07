#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "..\Character.h"
#include "..\..\GameSystem\SightSystem.h"
#include "..\..\GEKO\Task\FunctionTask.h"

//敵のステータス
struct EnemyState
{
	int flinch;
	float hp;
	float walkSpeed;
	float runSpeed;
	float mapHitRadius;
	float bodyRadius;
	float sightAngle;
	float sightDistance;
	Vector3D posSpawn;
	Vector3D rotation;
};

//ボーンのカプセル情報
struct BoneCalpule
{
	float radius;
	int start;
	int end;
	const char *name;
	BoneCalpule(float r, int s, int e, const char* name) :
		radius(r),
		start(s),
		end(e),
		name(name) {}
};

//基底の敵
class Enemy : public Character
{
public:
	Enemy(const char* name, EnemyState &enemyState);
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
	int m_FlinchNum; //耐久値
	float m_FlinchCnt; //耐久蓄積値
	float m_WalkSpeed; //歩く速度
	float m_RunSpeed; //走る速度

	//敵の視界
	EnemySight m_Sight; //視界システム
	SightData m_SightData; //視界のデータ
	Vector3D m_SightPos; //視界の位置
	Vector3D m_SightVec; //視界の方向

	//追跡用
	Vector3D m_Distance; //敵とプレイヤーとの距離
	const Vector3D *m_pPlayerPos; //プレイヤーの位置のポインタ

	//プレイヤーからの攻撃された時の当たり判定
	Collider *m_pCollider;
	CapsuleInfo *m_pCapsule;
	std::vector<BoneCalpule> m_BoneCapsule; //ボーン情報（カプセル）

	std::vector<float> m_DamageMagnification; //ダメージ倍率

	Collider *m_pHitAttack; //プレイヤーへの攻撃の当たり判定
	FunctionTask m_FuncTask; //AI管理
};

#endif