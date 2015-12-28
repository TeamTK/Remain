#include "Enemy.h"
#include "..\GameSystem\Effect.h"
#include "Player.h"

#define ENEMY_ANIM_END 29

Enemy::Enemy(Vector3D pos, Vector3D rot, const char* name) :
	Character(10, name, 1),
	m_FlinchNum(0),
	m_state(eState_Idle)
{
	m_SphereMap.radius = 0.2f; //ƒ}ƒbƒv‚Æ‚Ì”¼Œa
	m_BodyRadius = 0.5f; //“G‚Ì‘Ì‚Ì”¼Œa

	m_pos = pos;
	m_rot = rot;
	m_Model.SetAsset(name);

	//Ž‹ŠEƒVƒXƒeƒ€
	m_SightData.angle = 60.0f;
	m_SightData.distance = 10.0f;
	m_SightData.pSightPos = &m_pos;
	m_SightData.pSightVec = &m_SightVec;
	m_Sight.Regist(&m_SightData, REGIST_FUNC(Enemy::HitSight));
}

Enemy::~Enemy()
{
	unsigned int bornNum = BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++)
	{
		m_pCollider[i].Release();
	}
	BoneCapsule.clear();
	BoneCapsule.shrink_to_fit();

	delete[] m_pCollider;
	delete[] m_pCapsule;
}

void Enemy::Attack()
{
	m_Model.ChangeAnimation(eAnimationAttack);
	if (m_Model.GetPlayTime() == ENEMY_ANIM_END)
	{
		m_state = eState_Chase;
	}
}

void Enemy::Idle()
{
	m_Model.ChangeAnimation(eAnimationIdle);
}

void Enemy::Chase()
{
	//ƒvƒŒƒCƒ„[‚ð’ÇÕ
	m_Distance = (*m_pPlayerPos - m_pos);
	m_Model.ChangeAnimation(eAnimationTrot);
	m_rot = Vector3D::Lerp(m_rot, Vector3D(0.0f, atan2f(m_Distance.x, m_Distance.z), 0.0), 0.5f);

	float leng = m_Distance.LengthSq();

	//UŒ‚”»’f
	if (leng < 25)
	{
		m_pos += m_Distance.GetNormalize() * 0.07f;

		if (leng < 2)
		{
			m_state = eState_Attack;
			m_Model.SetTime(0);
		}
	}
	else
	{
		m_pos += m_Distance.GetNormalize() * 0.03f;
	}
}

void Enemy::HitDamage()
{
	if (m_Model.GetPlayTime() == ENEMY_ANIM_END)
	{
		m_state = eState_Chase;
	}
}

void Enemy::Die()
{
	m_Model.ChangeAnimation(eAnimationDie);
	m_Sight.Sleep();
	if (m_Model.GetPlayTime() == ENEMY_ANIM_END) Task::SetKill();
}

void Enemy::Update()
{
	//UŒ‚‚³‚ê‚é‘¤‚Ì“–‚½‚è”»’èXV
	unsigned int bornNum = BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++)
	{
		m_pCapsule[i].radius = BoneCapsule[i].radius;
		m_pCapsule[i].segment.start = m_Model.GetBornPos(BoneCapsule[i].start);
		m_pCapsule[i].segment.end = m_Model.GetBornPos(BoneCapsule[i].end);

	}
	m_SphereMap.pos = m_pos + Vector3D(0, m_SphereMap.radius, 0);

	m_SightVec = m_Model.GetAxisZ(1.0f);
	m_Model.SetPlayTime(30);

	//ó‘Ô‘JˆÚ
	switch (m_state)
	{
	case eState_Attack:
		Attack();
		break;

	case eState_Idle:
		Idle();
		break;

	case eState_Chase:
		Chase();
		break;

	case eState_HitDamage:
		HitDamage();
		break;

	case eState_Die:
		Die();

	default:
		break;
	}
	Character::Update();
}

void Enemy::HitBullet(Result_Sphere& r)
{
	//ŒŒ‚µ‚Ô‚«‚ÌƒGƒtƒFƒNƒg
	EffectInfo effectData;
	effectData.imageName = "Blood";
	effectData.num = 60;
	effectData.pos = r.position;
	effectData.scale = Vector3D(1.0f, 1.0f, 1.0f);
	effectData.speed = 0.1f;
	effectData.time = 120;
	EffectGeneration::Add(effectData);

	m_Hp--;
	m_FlinchNum++;
	if (m_Hp <= 0)
	{
		m_Model.SetTime(0);
		m_state = eState_Die;

		//“GŽ©g‚ÌUŒ‚‚Ì“–‚½‚è”»’è’âŽ~
		int colliderNum = BoneCapsule.size();
		for (int i = 0; i < colliderNum; i++) m_pCollider[i].Sleep();
	}
	else
	{
		if (m_FlinchNum >= 2)
		{
			m_FlinchNum = 0;
			m_state = eState_HitDamage;
			m_Model.ChangeAnimation(eAnimationHitDamage);
			m_Model.SetTime(0);
		}
		//m_Sight.Sleep();
	}

	if (!m_Sight.GetSleep())
	{
		m_pPlayerPos = g_pPlayerPos;
		m_state = eState_Chase;
		m_Sight.Sleep();
	}
}

void Enemy::HitSight(const Vector3D *pPos)
{
	m_pPlayerPos = pPos;
	m_state = eState_Chase;
	m_Sight.Sleep();
}