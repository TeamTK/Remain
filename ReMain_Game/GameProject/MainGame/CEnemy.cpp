#include "CEnemy.h"

CEnemy::CEnemy(int type,Vector3D pos) : CCharacter(type)
{
	m_state = eState_Idle;
	m_pos = pos;
	m_isChase = false;
	//Ž‹ŠEƒVƒXƒeƒ€
	m_SightData.angle = 60.0f;
	m_SightData.distance = 5.0f;
	m_SightData.pSightPos = &m_pos;
	m_SightData.pSightVec = &m_SightVec;
	m_Sight.Regist(&m_SightData, REGIST_FUNC(CEnemy::HitSight));
}

void CEnemy::Attack()
{
	m_Model.ChangeAnimation(eAnimationAttack);
	if (m_Model.GetPlayTime() == 29)
	{ 
		m_state = eState_Chase;
	}
}

void CEnemy::Idle() 
{
	m_Model.ChangeAnimation(eAnimationIdle);
}

void CEnemy::Chase()
{
	m_Distance = (*m_pPlayerPos - m_pos);
	m_Model.ChangeAnimation(eAnimationTrot);
	m_rot = Vector3D::Lerp(m_rot, Vector3D(0.0f, atan2f(m_Distance.x, m_Distance.z), 0.0), 0.5f);

	if (m_Distance.LengthSq() < 16)
	{
		m_pos += m_Distance.GetNormalize() * 0.05f;

		if (m_Distance.LengthSq() < 2)
		{
			m_state = eState_Attack;
		}
	}
	else
	{
		m_pos += m_Distance.GetNormalize() * 0.01f;
	}
}

void CEnemy::HitDamage()
{
	m_Model.ChangeAnimation(eAnimationHitDamage);

	if (m_Model.GetPlayTime() == 29)
	{
		if (!m_isChase) m_state = eState_Idle;
		else m_state = eState_Chase;
	}
}

void CEnemy::Die() 
{
	m_Model.ChangeAnimation(eAnimationDie);
	if (m_Model.GetPlayTime() == 29) m_isActive = false;
}

void CEnemy::Update() 
{
	m_SightVec = m_Model.GetAxisZ(1.0f);
	m_Model.SetPlayTime(30);

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
	CCharacter::Update();
}

void CEnemy::HitBullet() 
{
	m_Model.SetTime(0);
	m_Hp--;
	if(m_Hp <= 0)
	{
		m_state = eState_Die;
		int colliderNum = m_pCharaData->BoneCapsule.size();
		for (int i = 0; i < colliderNum; i++)
		{
			m_pCollider[i].Sleep();
		}
	} 
	else 
	{
		m_state = eState_HitDamage;
		//m_isChase = true;
		//m_Sight.Sleep();
	}
}

void CEnemy::HitSight(const Vector3D *pPos)
{
	m_pPlayerPos = pPos;
	m_state = eState_Chase;
	m_isChase = true;
	m_Sight.Sleep();
}

CEnemy *CEnemyManager::Add(int type,Vector3D pos)
{
	CEnemy *e = new CEnemy(type,pos);

	CCharacterManager::GetInstance()->Add(e);

	return e;
}