#include "Monster_A.h"
#include <vector>

#define MONSTER_A_ATTACK_ENDTIME 29
#define MONSTER_A_HITDAMAGE_ENDTIME 29
#define MONSTER_A_DIE_ENDTIME 28

#define MONSTER_A_ATTACK_SPEED 20.0f

Monster_A::Monster_A(EnemyState &enemyState) :
	Enemy("Monster_A", enemyState)
{
	//各部位のカプセルの情報(半径　ボーン番号　ボーン番号　部位名)
	m_BoneCapsule.emplace_back(0.4f, 25, 26, "Head");		  //頭
	m_BoneCapsule.emplace_back(0.6f, 2, 5, "Body");         //胴体
	m_BoneCapsule.emplace_back(0.5f, 6, 8, "Left arm");     //左腕
	m_BoneCapsule.emplace_back(0.5f, 16, 18, "Right arm");  //右腕
	m_BoneCapsule.emplace_back(0.5f, 27, 30, "Left leg");   //左足
	m_BoneCapsule.emplace_back(0.5f, 31, 34, "Right leg");  //右足

	//ダメージ倍率部位ごと
	m_DamageMagnification.push_back(2.0f); //頭
	m_DamageMagnification.push_back(1.0f); //胴体
	m_DamageMagnification.push_back(0.5f); //左腕
	m_DamageMagnification.push_back(0.5f); //右腕
	m_DamageMagnification.push_back(0.5f); //左足
	m_DamageMagnification.push_back(0.5f); //右足

	//球（弾）との判定用
	m_pHitAttackBody = new Collider[m_BoneCapsule.size()];
	m_pCapsule = new CapsuleInfo[m_BoneCapsule.size()];

	//攻撃の当たり判定
	m_pHitAttack = new Collider[m_BoneCapsule.size()];

	unsigned int bornNum = m_BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++)
	{
		m_pHitAttackBody[i].Regist_C_vs_S(&m_pCapsule[i].segment.start, &m_pCapsule[i].segment.end,
			&m_pCapsule[i].radius, REGIST_FUNC(Enemy::HitBullet));
		m_pHitAttackBody[i].SetID(eHITID0, eHITID1);
		m_pHitAttackBody[i].SetName(m_BoneCapsule[i].name);

		m_pHitAttack[i].Regist_C_vs_C(&m_pCapsule[i].segment.start, &m_pCapsule[i].segment.end,
			&m_pCapsule[i].radius, REGIST_FUNC(Enemy::HitAttack));
		m_pHitAttack[i].SetID(eHITID1, eHITID0);
		m_pHitAttack[i].Sleep();
	}

	//関数を登録
	m_FuncTask.Regist("Attack", REGIST_FUNC_TASK(Monster_A::Attack));
	m_FuncTask.Regist("Idle", REGIST_FUNC_TASK(Monster_A::Idle));
	m_FuncTask.Regist("Chase", REGIST_FUNC_TASK(Monster_A::Chase));
	m_FuncTask.Regist("HitDamage", REGIST_FUNC_TASK(Monster_A::HitDamage));
	m_FuncTask.Regist("Die", REGIST_FUNC_TASK(Monster_A::Die));
	m_FuncTask.Regist("Wandering", REGIST_FUNC_TASK(Monster_A::Wandering));

	//経路探索判断
	m_FuncTask.AllStop();
	if (enemyState.isSearch)
	{
		m_FuncTask.Start("Chase");
	}
	else
	{
		m_FuncTask.Start("Wandering");
	}

	m_MonsterType = eMonster_A;
}

Monster_A::~Monster_A()
{
}

void Monster_A::Attack()
{
	float animNum = m_Model.GetPlayTime();
	if(animNum >= 10 && animNum <= 11) m_pHitAttack[3].Awake(); //右腕の当たり判定起動
	if (animNum >= 20) m_pHitAttack[3].Sleep(); //右腕の当たり判定終了

	m_AnimType = eAnimationAttack;
	m_AnimSpeed = MONSTER_A_ATTACK_SPEED;
	m_AnimEndTime = MONSTER_A_ATTACK_ENDTIME;
	Enemy::Attack();
}

void Monster_A::Idle()
{
	m_AnimType = eAnimationIdle;
	Enemy::Idle();
}

void Monster_A::Chase()
{
	m_AnimType = eAnimationTrot;
	Enemy::Chase();
}

void Monster_A::HitDamage()
{
	m_AnimType = eAnimationHitDamage;
	m_AnimEndTime = MONSTER_A_HITDAMAGE_ENDTIME;
	Enemy::HitDamage();
}

void Monster_A::Die()
{
	m_AnimType = eAnimationDie;
	m_AnimEndTime = MONSTER_A_DIE_ENDTIME;
	m_AnimSpeed = ENEMY_NORMAL_SPEED;
	Enemy::Die();
}

void Monster_A::Wandering()
{
	Enemy::Wandering();

	//目的地に着いたら待機アニメーション
	if (m_Wandering.GetIsStopTime())
	{
		m_AnimType = eAnimationIdle;
	}
	else
	{
		m_AnimType = eAnimationTrot;
	}
}

void Monster_A::Update()
{
	Enemy::Update();
}