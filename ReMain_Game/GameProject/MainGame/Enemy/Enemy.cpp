#include "Enemy.h"
#include "..\..\GameSystem\Effect.h"
#include "..\Player.h"

#define COMMON_BORN_ANIM_ENEMY 20

Enemy::Enemy(const char* name, EnemyState &enemyState) :
	Character(10, name, 1),
	m_FlinchCnt(0.0f),
	m_AnimSpeed(ENEMY_NORMAL_SPEED)
{
	//各種設定値割り当て
	m_SphereMap.radius = enemyState.mapHitRadius; //マップとの半径
	m_BodyRadius = enemyState.bodyRadius; //敵の体の半径
	m_Hp = enemyState.hp;
	m_RunSpeed = enemyState.runSpeed;
	m_WalkSpeed = enemyState.walkSpeed;
	m_FlinchNum = enemyState.flinch;
	m_pos = enemyState.posSpawn;
	m_rot = enemyState.rotation;
	m_Model.SetAsset(name);

	//視界システム
	m_SightData.angle = enemyState.sightAngle;
	m_SightData.distance = enemyState.sightDistance;
	m_SightData.pSightPos = &m_SightPos;
	m_SightData.pSightVec = &m_SightVec;
	m_Sight.Regist(&m_SightData, REGIST_FUNC(Enemy::HitSight));
}

Enemy::~Enemy()
{
	unsigned int bornNum = m_BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++)
	{
		m_pHitAttackBody[i].Release();
	}
	m_BoneCapsule.clear();
	m_BoneCapsule.shrink_to_fit();

	delete[] m_pHitAttackBody;
	delete[] m_pCapsule;

	delete[] m_pHitAttack;
}

void Enemy::Attack()
{
	if (m_Model.GetPlayTime(COMMON_BORN_ANIM_ENEMY) >= m_AnimEndTime)
	{
		auto bornNum = m_BoneCapsule.size();
		for (unsigned int i = 0; i < bornNum; i++) m_pHitAttack[i].Sleep();
		m_FuncTask.Stop("Attack");
		m_FuncTask.Start("Chase");
	}
}

void Enemy::Idle()
{

}

void Enemy::Chase()
{
	//プレイヤーを追跡
	m_Distance = (*m_pPlayerPos - m_pos);
	m_rot = Vector3D(0.0f, atan2f(m_Distance.x, m_Distance.z), 0.0);

	float leng = m_Distance.LengthSq();

	//攻撃判断
	if (leng < 25)
	{
		m_pos += m_Distance.GetNormalize() * m_RunSpeed;

		if (leng < 2)
		{
			m_Model.SetTime(0);
			m_FuncTask.Stop("Chase");
			m_FuncTask.Start("Attack");
		}
	}
	else
	{
		m_pos += m_Distance.GetNormalize() * m_WalkSpeed;
	}
}

void Enemy::HitDamage()
{
	if (m_Model.GetPlayTime(m_JudgementAnim) >= m_AnimEndTime)
	{
		m_FuncTask.Stop("HitDamage");
		m_FuncTask.Start("Chase");
	}
}

void Enemy::Die()
{
	if (m_Model.GetPlayTime(m_JudgementAnim) >= m_AnimEndTime)
	{
		m_FuncTask.AllStop();
		Task::SetKill();
	}
}

void Enemy::Update()
{
	//デバック用停止機能
	static bool isStop = false;	
	if (Input::KeyI.Pressed()) isStop = true; //敵停止
	if (Input::KeyU.Pressed()) isStop = false; //敵停止

	if (!isStop)
	{
		//攻撃される側の当たり判定更新
		unsigned int bornNum = m_BoneCapsule.size();
		for (unsigned int i = 0; i < bornNum; i++)
		{
			m_pCapsule[i].radius = m_BoneCapsule[i].radius;
			m_pCapsule[i].segment.start = m_Model.GetBornPos(m_BoneCapsule[i].start);
			m_pCapsule[i].segment.end = m_Model.GetBornPos(m_BoneCapsule[i].end);

			if (!m_FuncTask.Running("Die")) m_pHitAttackBody[i].Awake();
		}
		m_SphereMap.pos = m_pos;
		m_SphereMap.pos.y += m_SphereMap.radius;

		//視界の更新
		m_SightPos = m_pos;
		m_SightPos.y += 2.0f;
		m_SightVec = m_Model.GetAxisZ(1.0f);

		m_Model.SetPlayTime(m_AnimSpeed);
		m_Model.ChangeAnimation(m_AnimType);

		m_FuncTask.Update();
		Character::Update();
	}
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

	//銃の種類ごとの威力
	float GunPower = 1.0f;
	if (r.name == "HandGun")
	{
		GunPower = 2.0f;
	}
	else if (r.name == "ShotGun")
	{
		GunPower = 3.0f;
	}
	
	std::cout << r.name << "\n";

	//当たり判定停止（弾からの）
	auto bornNum = m_BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++)
	{
		m_pHitAttackBody[i].Sleep();

		//部位ごとのダメージ計算
		if (r.name == m_pHitAttackBody[i].GetName())
		{
			float damegeNum = m_DamageMagnification[i] * GunPower;
			m_Hp -= damegeNum;
			m_FlinchCnt += damegeNum;
			std::cout << r.name << "\n";
		}
	}

	m_FuncTask.AllStop();

	//HPゼロになれば死亡
	if (m_Hp >= 0)
	{
		//怯み動作
		if (m_FlinchCnt >= (float)m_FlinchNum)
		{
			m_FlinchCnt = 0;
			m_Model.SetTime(0);
			m_FuncTask.Start("HitDamage");
			for (unsigned int i = 0; i < bornNum; i++) m_pHitAttack[i].Sleep();
		}
		else
		{
			m_FuncTask.Start("Chase");
		}

		//プレイヤーを発見していなくて攻撃を受けたら
		if (!m_Sight.GetSleep())
		{
			m_pPlayerPos = g_pPlayerPos;
			m_Sight.Sleep();
		}
	}
	else
	{
		for (unsigned int i = 0; i < bornNum; i++) m_pHitAttack[i].Sleep();
		m_Model.SetTime(0);
		m_FuncTask.Start("Die");
		m_Sight.Sleep();
	}
}

void Enemy::HitAttack(Result_Capsule &hitData)
{
	auto bornNum = m_BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++) m_pHitAttack[i].Sleep();
}

void Enemy::HitSight(const Vector3D *pPos)
{
	m_pPlayerPos = g_pPlayerPos;
	m_FuncTask.AllStop();
	m_FuncTask.Start("Chase");
	m_Sight.Sleep();
}