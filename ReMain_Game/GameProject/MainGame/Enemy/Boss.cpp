#include "Boss.h"
#include "Boss_Fluids.h"
#include "../Player/Player.h"
#include "../../GameSystem/Effect/EffectAnimation.h"
#include "../../GameSystem/Effect/SoundEffect.h"
#include "../Player/GameClear.h"
#include <random>

#define BOSS_ANIM_ENDTIME 28.0f

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
	m_MaxHp(bossState.hp),
	m_Hp(bossState.hp),
	m_FlinchNum(bossState.flinch),
	m_Pos(bossState.spawnPos),
	m_Rot(0.0f, 0.0f, 0.0f),
	m_FlinchCnt(0),
	m_PlayTime(0),
	m_Length(0),
	m_isDefence(false),
	m_isAlive(true)
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
	m_DamageMagnification.push_back(0.5f);	//茎
	m_DamageMagnification.push_back(1.5f);	//めしべ
	m_DamageMagnification.push_back(1.0f);	//花びら1
	m_DamageMagnification.push_back(1.0f);	//花びら2
	m_DamageMagnification.push_back(1.0f);	//花びら3
	m_DamageMagnification.push_back(1.0f);	//花びら4
	m_DamageMagnification.push_back(1.0f);	//花びら5
	m_DamageMagnification.push_back(0.8f);	//触手左
	m_DamageMagnification.push_back(0.8f);	//触手左
	m_DamageMagnification.push_back(0.8f);	//触手右
	m_DamageMagnification.push_back(0.8f);	//触手右

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
	m_FuncTask.Regist("Attack_A", REGIST_FUNC_TASK(Boss::Attack_A));
	m_FuncTask.Regist("Attack_B", REGIST_FUNC_TASK(Boss::Attack_B));
	m_FuncTask.Regist("Defence", REGIST_FUNC_TASK(Boss::Defence));
	m_FuncTask.Regist("Idle", REGIST_FUNC_TASK(Boss::Idle));
	m_FuncTask.Regist("HitDamage", REGIST_FUNC_TASK(Boss::HitDamage));
	m_FuncTask.Regist("Die", REGIST_FUNC_TASK(Boss::Die));
	m_FuncTask.Regist("LongAttack", REGIST_FUNC_TASK(Boss::LongAttack));
	m_FuncTask.AllStop();
	m_FuncTask.Start("Idle");

	m_pPlayerPos = g_pPlayerPos;
	m_NoActionTime.Start();
	m_Timer.Start();
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
	m_OneFlameTime = GEKO::GetOneFps();
	m_PlayTime = m_Model.GetPlayTime();

	//プレイヤーとの距離
	Vector3D distance = (*m_pPlayerPos - m_Pos);
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

	m_FuncTask.Update();
	m_Model.ChangeAnimation(m_AnimType);
	m_Model.SetPlayTime(m_AnimSpeed *m_OneFlameTime);

	//プレイヤーの方向に向く
	if (m_isAlive)
	{
		float rot = atan2f((m_Pos - *m_pPlayerPos).x, (m_Pos - *m_pPlayerPos).z);
		m_Model.SetRotationRadian(0.0f, rot, 0.0f);
	}
}

void Boss::Render()
{
	m_Model.Render();
}

void Boss::Attack_A()
{
	m_AnimType = eAnimationAttack1;
	m_AnimSpeed = BOSS_ATTACK_ANIMSPEED;

	if (m_PlayTime >= 19 && m_PlayTime <= 20) m_pHitAttack[7].Awake();
	if (m_PlayTime >= 19 && m_PlayTime <= 20) m_pHitAttack[8].Awake();
	if (m_PlayTime >= 19 && m_PlayTime <= 20) m_pHitAttack[9].Awake();
	if (m_PlayTime >= 19 && m_PlayTime <= 20) m_pHitAttack[10].Awake();
	if (m_PlayTime >= 22) m_pHitAttack[7].Sleep();
	if (m_PlayTime >= 22) m_pHitAttack[8].Sleep();
	if (m_PlayTime >= 22) m_pHitAttack[9].Sleep();
	if (m_PlayTime >= 22) m_pHitAttack[10].Sleep();

	if (m_PlayTime >= BOSS_ANIM_ENDTIME)
	{
		auto bornNum = m_BoneCapsule.size();
		for (unsigned int i = 0; i < bornNum; i++) m_pHitAttack[i].Sleep();
		m_FuncTask.Stop("Attac_A");
		m_FuncTask.Start("Idle");
	}
}

void Boss::Attack_B()
{
	m_AnimType = eAnimationAttack2;
	m_AnimSpeed = BOSS_ATTACK_ANIMSPEED;

	if (m_PlayTime >= 14 && m_PlayTime <= 15) m_pHitAttack[7].Awake();
	if (m_PlayTime >= 14 && m_PlayTime <= 15) m_pHitAttack[8].Awake();
	if (m_PlayTime >= 14 && m_PlayTime <= 15) m_pHitAttack[9].Awake();
	if (m_PlayTime >= 14 && m_PlayTime <= 15) m_pHitAttack[10].Awake();
	if (m_PlayTime >= 22) m_pHitAttack[7].Sleep();
	if (m_PlayTime >= 22) m_pHitAttack[8].Sleep();
	if (m_PlayTime >= 22) m_pHitAttack[9].Sleep();
	if (m_PlayTime >= 22) m_pHitAttack[10].Sleep();

	if (m_PlayTime >= BOSS_ANIM_ENDTIME)
	{
		auto bornNum = m_BoneCapsule.size();
		for (unsigned int i = 0; i < bornNum; i++) m_pHitAttack[i].Sleep();
		m_FuncTask.Stop("Attack_B");
		m_FuncTask.Start("Idle");
	}
}

void Boss::LongAttack()
{
	m_AnimType = eAnimLongAttack;
	m_AnimSpeed = BOSS_NORMAL_ANIMSPEED;

	if (m_PlayTime >= 12 && m_PlayTime <= 13)
	{
		Vector3D pos = m_Model.GetBornPos(14);
		new Boss_Fluids(pos, ((*m_pPlayerPos + Vector3D(0.0f, 1.0f, 0.0f)) - pos).GetNormalize(), 0.3f, 2.0f);
	}

	if (m_PlayTime >= BOSS_ANIM_ENDTIME)
	{
		m_FuncTask.Stop("LongAttack");
		m_FuncTask.Start("Idle");
	}
}

void Boss::Idle()
{
	m_AnimType = eAnimationIdle;
	m_AnimSpeed = BOSS_NORMAL_ANIMSPEED;
	/*
	if (m_Hp <= m_MaxHp * 0.3f)
	{
		m_Model.SetTime(0);
		m_FuncTask.Stop("Idle");
		m_FuncTask.Start("Defence");
		m_isDefence = true;
	}
	*/
	if (m_Length < 9.0f && m_Timer.GetSecond() > 3.0)
	{
		m_Timer.Start();
		m_Model.SetTime(0);
		m_Rot = m_Model.GetRotation();
		std::random_device rnd;
		std::mt19937 mt(rnd());
		std::uniform_int_distribution<> rand(0, 1);

		if (rand(mt) == 0)
		{
			m_FuncTask.Stop("Idle");
			m_FuncTask.Start("Attack_A");
		}
		else
		{
			m_FuncTask.Stop("Idle");
			m_FuncTask.Start("Attack_B");
		}
	}

	if (m_Length > 10.0f && m_NoActionTime.GetSecond() >= 2.0f)
	{
		if (m_Timer.GetSecond() > 3.0)
		{
			m_Timer.Start();
			m_Rot = m_Model.GetRotation();
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
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand(0, 1);

	//アニメーションを停止
	if (m_AnimType == eAnimationDefence && m_PlayTime >= 14)
	{
		m_Model.StopAnimation();
	}
	if (rand(mt) == 4)
	{
		m_isDefence = false;
	}
	if (!m_isDefence)
	{
		//アニメーションを再生
		m_Model.StartAnimation();
		if (m_AnimType == eAnimationDefence && m_PlayTime >= BOSS_ANIM_ENDTIME)
		{
			//m_Model.SetTime(0);
			m_FuncTask.Stop("Idle");
			m_FuncTask.Start("LongAttack");

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
	info.size = 2.0f;
	info.sizeW = 256;
	info.sizeH = 256;
	info.speed = 1.0f;
	new EffectAnimation("BloodAnim_Green", info);

	new SoundEffect("Blood", 3);

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
	}
	else
	{
		for (unsigned int i = 0; i < bornNum; i++)
			m_pHitAttack[i].Sleep();

		m_Model.SetTime(0);
		m_FuncTask.AllStop();
		m_FuncTask.Start("Die");
		m_isAlive = false;

		new GameClear();
	}
}

void Boss::HitAttack(Result_Capsule &hitData)
{
	auto bornNum = m_BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++)
		m_pHitAttack[i].Sleep();
}
