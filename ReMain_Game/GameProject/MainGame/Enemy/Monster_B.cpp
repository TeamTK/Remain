#include "Monster_B.h"

#define MONSTER_B_ATTACK_ENDTIME 29
#define MONSTER_B_HITDAMAGE_ENDTIME 29
#define MONSTER_B_DIE_ENDTIME 29

Monster_B::Monster_B(EnemyState &enemyState) :
	Enemy("Monster_B", enemyState)
{
	//各部位のカプセルの情報
	m_BoneCapsule.emplace_back(0.4f, 13, 14, "Head");		 //頭
	m_BoneCapsule.emplace_back(0.6f, 2, 13, "Body");	     //胴体
	m_BoneCapsule.emplace_back(0.5f, 6, 8, "Left arm");    //左腕
	m_BoneCapsule.emplace_back(0.5f, 10, 12, "Right arm"); //右腕
	m_BoneCapsule.emplace_back(0.5f, 15, 18, "Left leg");  //左足
	m_BoneCapsule.emplace_back(0.5f, 19, 22, "Rgiht leg"); //右足

	//ダメージ倍率部位ごと
	m_DamageMagnification.push_back(2.0f); //頭
	m_DamageMagnification.push_back(1.0f); //胴体
	m_DamageMagnification.push_back(0.5f); //左腕
	m_DamageMagnification.push_back(0.5f); //右腕
	m_DamageMagnification.push_back(0.5f); //左足
	m_DamageMagnification.push_back(0.5f); //右足

	//球（弾）との判定用
	m_pCollider = new Collider[m_BoneCapsule.size()];
	m_pCapsule = new CapsuleInfo[m_BoneCapsule.size()];

	//攻撃の判定
	m_pHitAttack = new Collider[m_BoneCapsule.size()];

	unsigned int bornNum = m_BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++)
	{
		m_pCollider[i].Regist_C_vs_S(&m_pCapsule[i].segment.start, &m_pCapsule[i].segment.end,
			&m_pCapsule[i].radius, REGIST_FUNC(Enemy::HitBullet));
		m_pCollider[i].SetID(eHITID0, eHITID1);
		m_pCollider[i].SetName(m_BoneCapsule[i].name);

		m_pHitAttack[i].Regist_C_vs_C(&m_pCapsule[i].segment.start, &m_pCapsule[i].segment.end,
			&m_pCapsule[i].radius, REGIST_FUNC(Enemy::HitAttack));
		m_pHitAttack[i].SetID(eHITID1, eHITID0);
		m_pHitAttack[i].Sleep();
	}

	//関数を登録
	m_FuncTask.Regist("Attack", REGIST_FUNC_TASK(Monster_B::Attack));
	m_FuncTask.Regist("Idle", REGIST_FUNC_TASK(Monster_B::Idle));
	m_FuncTask.Regist("Chase", REGIST_FUNC_TASK(Monster_B::Chase));
	m_FuncTask.Regist("HitDamage", REGIST_FUNC_TASK(Monster_B::HitDamage));
	m_FuncTask.Regist("Die", REGIST_FUNC_TASK(Monster_B::Die));

	m_FuncTask.AllStop();
	m_FuncTask.Start("Idle");
}

Monster_B::~Monster_B()
{
}

void Monster_B::Attack()
{
	//抱きしめ攻撃当たり判定開始
	if (m_Model.GetPlayTime() == 10)
	{
		m_pHitAttack[2].Awake(); //左腕の当たり判定起動
		m_pHitAttack[3].Awake(); //右腕の当たり判定起動
	}

	//抱きしめ攻撃当たり判定終了
	if (m_Model.GetPlayTime() == 20)
	{
		m_pHitAttack[2].Sleep(); //左腕の当たり判定起動
		m_pHitAttack[3].Sleep(); //右腕の当たり判定起動
	}

	m_AnimType = eAnimationAttack;
	m_AnimEndTime = MONSTER_B_ATTACK_ENDTIME;
	Enemy::Attack();
}

void Monster_B::Idle()
{
	m_AnimType = eAnimationIdle;
	Enemy::Idle();
}

void Monster_B::Chase()
{
	m_AnimType = eAnimationTrot;
	Enemy::Chase();
}

void Monster_B::HitDamage()
{
	m_AnimType = eAnimationHitDamage;
	m_AnimEndTime = MONSTER_B_HITDAMAGE_ENDTIME;

	Enemy::HitDamage();
}

void Monster_B::Die()
{
	m_AnimType = eAnimationDie;
	m_AnimEndTime = MONSTER_B_DIE_ENDTIME;
	m_AnimSpeed = ENEMY_NORMAL_SPEED;
	Enemy::Die();
}

void Monster_B::Update()
{
	Enemy::Update();
}