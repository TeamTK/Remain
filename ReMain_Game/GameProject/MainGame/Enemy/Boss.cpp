#include "Boss.h"
#include "Boss_Fluids.h"
#include "../Player/Player.h"
#include "../../GameSystem/Effect/EffectAnimation.h"
#include <random>

#define BOSS_ANIM_ENDTIME 28.5f

#define BOSS_ATTACK_ANIMSPEED 20.0f
#define BOSS_NORMAL_ANIMSPEED 30.0f
#define BOSS_DIE_ANIMSPEED 20.0f

#define GUN_POWER_HANDGUN 2.0f //ショットガンの威力
#define GUN_POWER_SHOTGUN 4.0f //拳銃の威力
#define DEFENCE_MIGRATION_TIME 1.5f

Boss::Boss(BossState &bossState) :
	Task("Boss", 1),
	m_AnimType(eAnimationIdle),
	m_AnimSpeed(BOSS_NORMAL_ANIMSPEED),
	m_Hp(bossState.hp),
	m_FlinchNum(bossState.flinch),
	m_FlinchCnt(0),
	m_PlayTime(0),
	m_Length(0),
	m_Timer(0),
	m_Rand(0),
	m_Cnt(0),
	m_isDefence(false)
{
	m_RenderTask.Regist(1, REGIST_RENDER_FUNC(Boss::Render));

	m_Model.SetAsset("Boss", true);
	m_Model.SetTranselate(bossState.spawnPos);
	m_Model.SetRotationDegree(0, (int)bossState.spawnRot.y, 0);

	m_BoneCapsule.emplace_back(0.5f, 1, 5, "Stem");		  //茎
	m_BoneCapsule.emplace_back(1.0f, 13, 14, "Pistil");	  //めしべ
	m_BoneCapsule.emplace_back(0.8f, 16, 18, "Petals1");  //花びら1
	m_BoneCapsule.emplace_back(0.8f, 19, 21, "Petals2");  //花びら2
	m_BoneCapsule.emplace_back(0.8f, 22, 24, "Petals3");  //花びら3
	m_BoneCapsule.emplace_back(0.8f, 25, 27, "Petals4");  //花びら4
	m_BoneCapsule.emplace_back(0.8f, 28, 30, "Petals5");  //花びら5
	m_BoneCapsule.emplace_back(0.8f, 32, 35, "Tentacles_L1");  //触手左
	m_BoneCapsule.emplace_back(0.8f, 36, 39, "Tentacles_L2");  //触手左
	m_BoneCapsule.emplace_back(0.8f, 41, 44, "Tentacles_R1");  //触手右
	m_BoneCapsule.emplace_back(0.8f, 45, 48, "Tentacles_R2");  //触手右

	//ダメージ倍率
	m_DamageMagnification.push_back(0.8f);	//茎
	m_DamageMagnification.push_back(2.0f);	//めしべ
	m_DamageMagnification.push_back(0.8f);	//花びら1
	m_DamageMagnification.push_back(0.8f);	//花びら2
	m_DamageMagnification.push_back(0.8f);	//花びら3
	m_DamageMagnification.push_back(0.8f);	//花びら4
	m_DamageMagnification.push_back(0.8f);	//花びら5
	m_DamageMagnification.push_back(0.5f);	//触手左
	m_DamageMagnification.push_back(0.5f);	//触手左
	m_DamageMagnification.push_back(0.5f);	//触手右
	m_DamageMagnification.push_back(0.5f);	//触手右

	 //球（弾）との判定用
	m_pHitAttackBody = new Collider[m_BoneCapsule.size()];
	m_pCapsule = new CapsuleInfo[m_BoneCapsule.size()];

	//自身の攻撃の当たり判定
	m_pHitAttack = new Collider[m_BoneCapsule.size()];

	unsigned int bornNum = m_BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++)
	{
		m_pHitAttackBody[i].Regist_C_vs_S(&m_pCapsule[i].segment.start, &m_pCapsule[i].segment.end, &m_pCapsule[i].radius, REGIST_FUNC(Boss::HitBullet));
		m_pHitAttackBody[i].SetID(eHITID0, eHITID1);
		m_pHitAttackBody[i].SetName(m_BoneCapsule[i].name);

		m_pHitAttack[i].Regist_C_vs_C(&m_pCapsule[i].segment.start, &m_pCapsule[i].segment.end, &m_pCapsule[i].radius, REGIST_FUNC(Boss::HitAttack));
		m_pHitAttack[i].SetID(eHITID1, eHITID0);
		m_pHitAttack[i].Sleep();
	}

	//関数を登録
	m_FuncTask.Regist("Attack", REGIST_FUNC_TASK(Boss::Attack));
	m_FuncTask.Regist("Defence", REGIST_FUNC_TASK(Boss::Defence));
	m_FuncTask.Regist("Idle", REGIST_FUNC_TASK(Boss::Idle));
	m_FuncTask.Regist("HitDamage", REGIST_FUNC_TASK(Boss::HitDamage));
	m_FuncTask.Regist("Die", REGIST_FUNC_TASK(Boss::Die));
	m_FuncTask.Regist("LongAttack", REGIST_FUNC_TASK(Boss::LongAttack));
	m_FuncTask.AllStop();
	m_FuncTask.Start("Idle");

	m_pos = bossState.spawnPos;
	m_pPlayerPos = g_pPlayerPos;
	m_NoActionTime.Start();
}

Boss::~Boss()
{
	m_BoneCapsule.clear();
	m_BoneCapsule.shrink_to_fit();
	m_DamageMagnification.clear();
	m_DamageMagnification.shrink_to_fit();
	delete[] m_pHitAttackBody;
	delete[] m_pCapsule;

	delete[] m_pHitAttack;
}

void Boss::Update()
{
	//1フレームタイム
	m_OneFlameTime = GEKO::GetOneFps();
	//アニメーション再生中フレーム
	m_PlayTime = m_Model.GetPlayTime();

	//プレイヤーとの距離
	Vector3D distance = (*m_pPlayerPos - m_pos);
	m_Length = distance.Length();

	//攻撃される側の当たり判定更新
	unsigned int bornNum = m_BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++)
	{
		m_pCapsule[i].radius = m_BoneCapsule[i].radius;
		m_pCapsule[i].segment.start = m_Model.GetBornPos(m_BoneCapsule[i].start);
		m_pCapsule[i].segment.end = m_Model.GetBornPos(m_BoneCapsule[i].end);

		if (!m_FuncTask.Running("Die")) m_pHitAttackBody[i].Awake();
	}
	//printf("%f\n", m_Hp);

	//ランダム
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand(0, 39);
	m_Rand = rand(mt);

	m_FuncTask.Update();
	m_Model.ChangeAnimation(m_AnimType);
	m_Model.SetPlayTime(m_AnimSpeed *m_OneFlameTime);
}

void Boss::Render()
{
	m_Model.Render();
}

void Boss::Attack()
{
	m_AnimType = eAnimationAttack2;
	m_AnimSpeed = BOSS_ATTACK_ANIMSPEED;

	float animNum = m_Model.GetPlayTime();
	if (m_PlayTime >= 12 && m_PlayTime <= 13) m_pHitAttack[8].Awake(); //右腕の当たり判定起動
	if (m_PlayTime >= 22) m_pHitAttack[8].Sleep();			//右腕の当たり判定終了

	if (m_PlayTime >= 12 && m_PlayTime <= 13) m_pHitAttack[7].Awake(); //左腕の当たり判定起動
	if (m_PlayTime >= 22) m_pHitAttack[7].Sleep();			//左腕の当たり判定終了

	if (m_PlayTime >= BOSS_ANIM_ENDTIME)
	{
		auto bornNum = m_BoneCapsule.size();
		for (unsigned int i = 0; i < bornNum; i++) m_pHitAttack[i].Sleep();
		m_Model.SetTime(0);
		m_FuncTask.Stop("Attack");
		m_FuncTask.Start("Idle");
	}
}


void Boss::LongAttack()
{
	m_AnimType = eAnimationIdle;
	m_AnimSpeed = BOSS_NORMAL_ANIMSPEED;

	if (m_PlayTime >= 12 && m_PlayTime <= 13)
	{
		Vector3D pos = m_Model.GetBornPos(14);
		new Boss_Fluids(pos, ((*m_pPlayerPos + Vector3D(0.0f, 1.0f, 0.0f)) - pos).GetNormalize(), 0.4f, 1.5f);
	}

	if (m_PlayTime >= BOSS_ANIM_ENDTIME)
	{
		m_Model.SetTime(0);
		m_FuncTask.Stop("LongAttack");
		m_FuncTask.Start("Idle");
	}
}

void Boss::Idle()
{
	m_AnimType = eAnimationIdle;
	m_AnimSpeed = BOSS_NORMAL_ANIMSPEED;

	if (m_Length < 8.0f)
	{
		if (m_Hp < m_Hp * 0.3f)
		{
			m_Model.SetTime(0);
			m_FuncTask.Stop("Idle");
			m_FuncTask.Start("Defence");
		}
		else
		{
			if (m_NoActionTime.GetSecond() >= 2.0f)
				if (m_Rand == 2)
				{
					m_Model.SetTime(0);
					m_FuncTask.Stop("Idle");
					m_FuncTask.Start("Attack");
				}
		}
	}

	if (m_Length > 14.0f)
		m_Timer += GEKO::GetOneFps();
	else
		m_Timer = 0.0f;

	if (m_Timer >= DEFENCE_MIGRATION_TIME)
	{
		if (m_Rand == 0)
		{
			m_Timer = 0.0f;
			m_Model.SetTime(0);
			m_FuncTask.Stop("Idle");
			m_FuncTask.Start("LongAttack");
		}
	}
}

void Boss::Defence()
{
	m_AnimType = eAnimationDefence;
	m_AnimSpeed = BOSS_NORMAL_ANIMSPEED;

	//アニメーションを停止
	if (m_AnimType == eAnimationDefence && m_PlayTime >= 14)
	{
		m_Model.StopAnimation();
	}
	if (m_Rand == 1)m_isDefence = false;

	if (!m_isDefence)
	{
		//アニメーションを再生
		m_Model.StartAnimation();
		if (m_AnimType == eAnimationDefence && m_PlayTime >= BOSS_ANIM_ENDTIME)
		{
			m_Model.SetTime(0);
			m_FuncTask.Stop("Defence");
			m_FuncTask.Start("Idle");

			//当たり判定を有効
			auto bornNum = m_BoneCapsule.size();
			for (unsigned int i = 0; i < bornNum; i++)
			{
				m_pHitAttackBody[i].Awake();
			}
		}
	}
	else
	{
		//当たり判定を停止
		auto bornNum = m_BoneCapsule.size();
		for (unsigned int i = 0; i < bornNum; i++)
		{
			m_pHitAttackBody[i].Sleep();
		}
	}
}


void Boss::HitDamage()
{
	m_AnimType = eAnimationHitDamage;
	m_AnimSpeed = BOSS_NORMAL_ANIMSPEED;

	if (m_PlayTime >= BOSS_ANIM_ENDTIME)
	{
		m_Model.SetTime(0);
		m_FuncTask.Stop("HitDamage");
		m_FuncTask.Start("Idle");
	}
}

void Boss::Die()
{
	m_AnimType = eAnimationDie;
	m_AnimSpeed = BOSS_DIE_ANIMSPEED;

	if (m_PlayTime >= BOSS_ANIM_ENDTIME)
	{
		m_Model.StopAnimation();
	}
}


void Boss::HitBullet(Result_Sphere& r)
{
	//血しぶきのエフェクト
	EffectAnimationInfo info;
	info.frameNum = 8;
	info.pos = r.position;
	info.size = 1.0f;
	info.sizeW = 256;
	info.sizeH = 256;
	info.speed = 1.0f;
	new EffectAnimation("BloodAnim_Green", info);

	float GunPower = 1.0f;
	if (r.targetName == "HandGun")
	{
		GunPower = GUN_POWER_HANDGUN;
	}
	else if (r.targetName == "ShotGun")
	{
		GunPower = GUN_POWER_SHOTGUN;
	}

	auto bornNum = m_BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++)
	{
		//当たり判定を停止
		m_pHitAttackBody[i].Sleep();

		//ダメージ倍率
		if (r.name == m_pHitAttackBody[i].GetName())
		{
			float damegeNum = m_DamageMagnification[i] * GunPower;
			m_Hp -= damegeNum;
			m_FlinchCnt += damegeNum;
		}
	}

	if (m_Hp >= 0.0f)
	{
		//怯み動作
		if (m_FlinchCnt >= (float)m_FlinchNum)
		{
			m_Model.SetTime(0);
			m_FuncTask.Start("HitDamage");
			m_FlinchCnt = 0;
			for (unsigned int i = 0; i < bornNum; i++) m_pHitAttack[i].Sleep();
		}
		else
		{
			m_Model.SetTime(0);
			m_FuncTask.Start("Idle");
		}
	}
	else
	{
		for (unsigned int i = 0; i < bornNum; i++)
			m_pHitAttack[i].Sleep();

		m_Model.SetTime(0);
		m_FuncTask.AllStop();
		m_FuncTask.Start("Die");
	}
}

void Boss::HitAttack(Result_Capsule &hitData)
{
	auto bornNum = m_BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++)
		m_pHitAttack[i].Sleep();
}
