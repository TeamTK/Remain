#include "Monster_B.h"

#define MONSTER_B_ATTACK_ENDTIME 29
#define MONSTER_B_HITDAMAGE_ENDTIME 29
#define MONSTER_B_DIE_ENDTIME 29

Monster_B::Monster_B(EnemyState &enemyState) :
	Enemy("Monster_B", enemyState)
{
	//各部位のカプセルの情報
	m_BoneCapsule.emplace_back(0.4f, 15, 16, "Head");		 //頭
	m_BoneCapsule.emplace_back(0.6f, 2, 5, "Body");	     //胴体
	m_BoneCapsule.emplace_back(0.5f, 6, 8, "Left arm");    //左腕
	m_BoneCapsule.emplace_back(0.5f, 10, 13, "Right arm"); //右腕
	m_BoneCapsule.emplace_back(0.5f, 17, 19, "Left leg");  //左足
	m_BoneCapsule.emplace_back(0.5f, 21, 23, "Rgiht leg"); //右足

	//ダメージ倍率部位ごと
	m_DamageMagnification.push_back(4.0f); //頭
	m_DamageMagnification.push_back(2.5f); //胴体
	m_DamageMagnification.push_back(1.0f); //左腕
	m_DamageMagnification.push_back(1.0f); //右腕
	m_DamageMagnification.push_back(1.0f); //左足
	m_DamageMagnification.push_back(1.0f); //右足

	//球（弾）との判定用
	m_pHitAttackBody = new Collider[m_BoneCapsule.size()];
	m_pCapsule = new CapsuleInfo[m_BoneCapsule.size()];

	//攻撃の当たり判定
	m_pHitAttack = new Collider[m_BoneCapsule.size()];

	unsigned int bornNum = m_BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++)
	{
		//プレイヤーからの攻撃の当たり判定
		m_pHitAttackBody[i].Regist_C_vs_S(&m_pCapsule[i].segment.start, &m_pCapsule[i].segment.end,
			&m_pCapsule[i].radius, REGIST_FUNC(Enemy::HitBullet));
		m_pHitAttackBody[i].SetID(eHITID0, eHITID1);
		m_pHitAttackBody[i].SetName(m_BoneCapsule[i].name);

		//敵からの攻撃の当たり判定
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

	//経路探索判断
	m_FuncTask.AllStop();
	if (enemyState.isSearch)
	{
		m_FuncTask.Start("Chase");
	}
	else
	{
		m_FuncTask.Start("Idle");
	}

	m_JudgementAnim = 13;

	m_DamegeBlood.SetAsset("Monster_B_2");
}

Monster_B::~Monster_B()
{
}

void Monster_B::Attack()
{
	float animNum = m_Model.GetPlayTime(13);

	//抱きしめ攻撃当たり判定開始
	if (animNum >= 10)
	{
		m_pHitAttack[2].Awake(); //左腕の当たり判定起動
		m_pHitAttack[3].Awake(); //右腕の当たり判定起動
	}

	//抱きしめ攻撃当たり判定終了
	if (animNum >= 20)
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