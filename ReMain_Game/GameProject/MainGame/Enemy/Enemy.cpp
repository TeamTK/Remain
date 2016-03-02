#include "Enemy.h"
#include "../Player/Player.h"
#include "../../GameSystem/Effect/EffectAnimation.h"
#include "../AmmoBox/AmmoBox_Shotgun.h"
#include "../AmmoBox/AmmoBox_Handgun.h"
#include <random>

#define SEARCH_INTERVAL 60 //経路探索開始間隔
#define DROP_PROBABILITY 2 //アイテムドロップ率
#define GUN_POWER_HANDGUN 2.0f //ショットガンの威力
#define GUN_POWER_SHOTGUN 5.0f //拳銃の威力

Enemy::Enemy(const char* name, EnemyState &enemyState) :
	Character(10, name, 1),
	m_FlinchCnt(0.0f),
	m_AnimSpeed(ENEMY_NORMAL_SPEED),
	m_SearchCnt(0),
	m_OneFlameTime(0)
{
	//各種設定値割り当て
	m_SphereMap.radius = enemyState.mapHitRadius; //マップとの半径
	m_BodyRadius = enemyState.bodyRadius; //敵の体の半径
	m_Hp = enemyState.hp;
	m_RunSpeed = enemyState.runSpeed;
	m_WalkSpeed = enemyState.walkSpeed;
	m_FlinchNum = enemyState.flinch;
	m_AuditoryRange = 10.0f;
	m_pos = enemyState.posSpawn;
	m_rot = enemyState.rotation;
	m_Model.SetAsset(name, true);

	//視界システム
	m_SightData.angle = enemyState.sightAngle;
	m_SightData.distance = enemyState.sightDistance;
	m_SightData.pSightPos = &m_SightPos;
	m_SightData.pSightVec = &m_SightVec;
	m_Sight.Regist(&m_SightData, REGIST_FUNC(Enemy::HitSight));

	//聴覚探知
	m_AuditorySense.SetFunc(REGIST_FUNC(Enemy::Auditory));
	m_AuditorySense.SetPos(&m_pos);
	m_AuditorySense.SetDetectionRange(&m_AuditoryRange);

	//経路探索
	m_Search.SetPosition(&m_pos);
	m_Search.SetTargetName("Player");
	m_Search.SetTopography(enemyState.topographyName);

	//経路探索開始判断
	if (enemyState.isSearch)
	{
		m_Search.StartSerch();
		m_Search.StartMove();
		m_Sight.Sleep();
		m_AuditorySense.Stop();
		m_pPlayerPos = g_pPlayerPos;
	}
	else
	{
		m_Search.StopSerch();
		m_Search.StopMove();
	}

	//徘徊初期化
	m_Wandering.Init(enemyState.wanderingName, &m_Search, &m_pos, &m_rot);
}

Enemy::~Enemy()
{
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
	float leng = m_Distance.LengthSq();

	//攻撃判断(経路探索でゴールしたら)
	if (m_Search.GetIsGoal())
	{
		m_pos += m_Distance.GetNormalize() * m_RunSpeed * m_OneFlameTime;
		m_rot = Vector3D(0.0f, atan2f(m_Distance.x, m_Distance.z), 0.0);

		if (leng < 3)
		{
			m_Model.SetTime(0);
			m_FuncTask.Stop("Chase");
			m_FuncTask.Start("Attack");
		}
		m_Search.StopMove();
	}
	else
	{
		m_Search.StartMove();
		m_Search.StartSerch();

		m_pos += *m_Search.GetTargetDirection() * m_WalkSpeed * m_OneFlameTime;
		m_rot = Vector3D(0.0f, atan2f(m_Search.GetTargetDirection()->x, m_Search.GetTargetDirection()->z), 0.0);
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
		std::random_device rnd;     // 非決定的な乱数生成器を生成
		std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
		std::uniform_int_distribution<> rand(0, 1);
		std::uniform_int_distribution<> rand2(0, DROP_PROBABILITY);

		//弾をドロップ（5分の1）
		if (rand2(mt) == 0)
		{
			switch (rand(mt))
			{
			case 0:
				new AmmoBox_Shotgun(m_pos, Vector3D(0.0f, 0.0f, 0.0f), 7);
				break;

			case 1:
				new AmmoBox_Handgun(m_pos, Vector3D(0.0f, 0.0f, 0.0f), 6);
				break;

			default:
				break;
			}
		}

		m_Model.StopAnimation();
		m_FuncTask.AllStop();
		Task::SetKill();
	}
}

void Enemy::Wandering()
{
	m_Wandering.Update();
}

void Enemy::Update()
{
	m_OneFlameTime = GEKO::GetOneFps();
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

	//アニメーション切り替えと速度
	m_Model.SetPlayTime(m_AnimSpeed * m_OneFlameTime);
	m_Model.ChangeAnimation(m_AnimType);

	m_FuncTask.Update();
	m_FuncTask.RunningDraw();

	//経路探索をする間隔
	if (m_SearchCnt >= SEARCH_INTERVAL && m_Sight.GetSleep())
	{
		m_Search.StartSerch();
		m_SearchCnt = 0;
	}
	else
	{
		m_Search.StopSerch();
		m_SearchCnt++;
	}
	Character::Update();
	//m_Search.DebugMigrationPath();
}

void Enemy::HitBullet(Result_Sphere& r)
{
	//血しぶきのエフェクト
	EffectAnimationInfo info;
	info.frameNum = 8;
	info.pos = r.position;
	info.size = 2.0f;
	info.sizeW = 256;
	info.sizeH = 256;
	info.speed = 1.0f;
	new EffectAnimation("BloodAnim", info);

	m_Model.SetTexture(&m_DamegeBlood);

	//銃の種類ごとの威力
	float GunPower = 1.0f;
	if (r.targetName == "HandGun")
	{
		GunPower = GUN_POWER_HANDGUN;
	}
	else if (r.targetName == "ShotGun")
	{
		GunPower = GUN_POWER_SHOTGUN;
	}

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
		}
	}

	m_FuncTask.AllStop();

	//HPゼロになれば死亡
	if (m_Hp >= 0)
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
			m_FuncTask.Start("Chase");
		}

		//プレイヤーを発見していなくて攻撃を受けたら
		if (!m_Sight.GetSleep())
		{
			m_pPlayerPos = g_pPlayerPos;
			m_Sight.Sleep();
			m_AuditorySense.Stop();
			m_Search.StartSerch();
			m_Search.StartMove();
			m_Wandering.Stop();
		}
	}
	else
	{
		for (unsigned int i = 0; i < bornNum; i++) m_pHitAttack[i].Sleep();
		m_Model.SetTime(0);
		m_FuncTask.AllStop();
		m_FuncTask.Start("Die");
		m_Sight.Sleep();
		m_AuditorySense.Stop();
		m_Wandering.Stop();
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
	m_AuditorySense.Stop();
	m_Search.StartSerch();
	m_Search.StartMove();
	m_Wandering.Stop();
}

void Enemy::Auditory(int volume)
{
	switch (volume)
	{
	case eOblivious:
		break;

	case eCaution:
		break;

	case eDiscovery:
		m_pPlayerPos = g_pPlayerPos;
		m_FuncTask.AllStop();
		m_FuncTask.Start("Chase");
		m_Sight.Sleep();
		m_AuditorySense.Stop();
		m_Search.StartSerch();
		m_Search.StartMove();
		m_Wandering.Stop();
		break;

	default:
		break;
	}
}