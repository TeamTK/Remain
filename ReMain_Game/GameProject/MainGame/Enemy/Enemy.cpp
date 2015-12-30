#include "Enemy.h"
#include "..\..\GameSystem\Effect.h"
#include "..\Player.h"

#define ENEMY_ANIM_END 29
#define ENEMY_

Enemy::Enemy(Vector3D pos, Vector3D rot, const char* name) :
	Character(10, name, 1),
	m_FlinchNum(0)
{
	m_SphereMap.radius = 0.2f; //マップとの半径
	m_BodyRadius = 0.5f; //敵の体の半径

	m_pos = pos;
	m_rot = rot;
	m_Model.SetAsset(name);

	//視界システム
	m_SightData.angle = 60.0f;
	m_SightData.distance = 10.0f;
	m_SightData.pSightPos = &m_SightPos;
	m_SightData.pSightVec = &m_SightVec;
	m_Sight.Regist(&m_SightData, REGIST_FUNC(Enemy::HitSight));
}

Enemy::~Enemy()
{
	unsigned int bornNum = m_BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++)
	{
		m_pCollider[i].Release();
	}
	m_BoneCapsule.clear();
	m_BoneCapsule.shrink_to_fit();

	delete[] m_pCollider;
	delete[] m_pCapsule;

	delete[] m_pHitAttack;
}

void Enemy::Attack(unsigned int animNum)
{
	m_Model.ChangeAnimation(animNum);
	if (m_Model.GetPlayTime() == ENEMY_ANIM_END)
	{
		m_FuncTask.Sleep("Attack");
		m_FuncTask.Awake("Chase");
	}
}

void Enemy::Idle(unsigned int animNum)
{
	m_Model.ChangeAnimation(animNum);
}

void Enemy::Chase(unsigned int animNum)
{
	//プレイヤーを追跡
	m_Distance = (*m_pPlayerPos - m_pos);
	m_Model.ChangeAnimation(animNum);
	m_rot = Vector3D(0.0f, atan2f(m_Distance.x, m_Distance.z), 0.0);

	float leng = m_Distance.LengthSq();

	//攻撃判断
	if (leng < 25)
	{
		m_pos += m_Distance.GetNormalize() * 0.07f;

		if (leng < 2)
		{
			m_Model.SetTime(0);
			m_pHitAttack[3].Awake();
			m_FuncTask.Sleep("Chase");
			m_FuncTask.Awake("Attack");
		}
	}
	else
	{
		m_pos += m_Distance.GetNormalize() * 0.03f;
	}
}

void Enemy::HitDamage(unsigned int animNum)
{
	m_Model.ChangeAnimation(animNum);

	if (m_Model.GetPlayTime() == ENEMY_ANIM_END)
	{
		m_FuncTask.Sleep("HitDamage");
		m_FuncTask.Awake("Chase");
	}
}

void Enemy::Die(unsigned int animNum)
{
	m_Model.ChangeAnimation(animNum);
	m_Sight.Sleep();
	if (m_Model.GetPlayTime() == ENEMY_ANIM_END)
	{
		m_FuncTask.Sleep("Die");
		Task::SetKill();
	}
}

void Enemy::Update()
{
	//攻撃される側の当たり判定更新
	unsigned int bornNum = m_BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++)
	{
		m_pCapsule[i].radius = m_BoneCapsule[i].radius;
		m_pCapsule[i].segment.start = m_Model.GetBornPos(m_BoneCapsule[i].start);
		m_pCapsule[i].segment.end = m_Model.GetBornPos(m_BoneCapsule[i].end);
	}
	m_SphereMap.pos = m_pos;
	m_SphereMap.pos.y += m_SphereMap.radius;

	m_SightPos = m_pos;
	m_SightPos.y += 2.0f;

	m_SightVec = m_Model.GetAxisZ(1.0f);
	m_Model.SetPlayTime(30);

	m_FuncTask.Update();

	Character::Update();
}

void Enemy::HitBullet(Result_Sphere& r)
{
	//血しぶきのエフェクト
	EffectInfo effectData;
	effectData.imageName = "Blood";
	effectData.num = 60;
	effectData.pos = r.position;
	effectData.scale = Vector3D(1.0f, 1.0f, 1.0f);
	effectData.speed = 0.1f;
	effectData.time = 120;
	new Effect(effectData, "Blood");

	m_Hp--;
	m_FlinchNum++;
	if (m_Hp <= 0)
	{
		m_Model.SetTime(0);
		m_FuncTask.AllSleep();
		m_FuncTask.Awake("Die");

		//敵自身の攻撃の当たり判定停止
		int colliderNum = m_BoneCapsule.size();
		for (int i = 0; i < colliderNum; i++) m_pCollider[i].Sleep();
	}
	else
	{
		m_FuncTask.AllSleep();
		m_Model.SetTime(0);

		if (m_FlinchNum >= 2)
		{
			m_FlinchNum = 0;
			m_FuncTask.Awake("HitDamage");
		}
		else
		{
			m_FuncTask.Awake("Chase");
		}

		//プレイヤーを発見していなくて攻撃を受けたら
		if (!m_Sight.GetSleep())
		{
			m_pPlayerPos = g_pPlayerPos;
			m_Sight.Sleep();
		}
	}
}

void Enemy::HitAttack(Result_Capsule &hitData)
{
	unsigned int bornNum = m_BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++)
	{
		m_pHitAttack[i].Sleep();
	}
}

void Enemy::HitSight(const Vector3D *pPos)
{
	m_pPlayerPos = g_pPlayerPos;
	m_FuncTask.AllSleep();
	m_FuncTask.Awake("Chase");
	m_Sight.Sleep();
}