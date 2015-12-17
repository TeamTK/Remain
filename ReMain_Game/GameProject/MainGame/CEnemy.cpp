#include "CEnemy.h"

CEnemy::CEnemy(int type, Vector3D pos, Vector3D rot) :
	CCharacter(type),
	m_isChase(false),
	m_FlinchNum(0),
	m_state(eState_Idle)
{
	m_pos = pos;
	m_rot = rot;

	//éãäEÉVÉXÉeÉÄ
	m_SightData.angle = 60.0f;
	m_SightData.distance = 5.0f;
	m_SightData.pSightPos = &m_pos;
	m_SightData.pSightVec = &m_SightVec;
	m_Sight.Regist(&m_SightData, REGIST_FUNC(CEnemy::HitSight));

	m_BodyRadius = 0.5f; //ìGÇÃëÃÇÃîºåa
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
	//ÉvÉåÉCÉÑÅ[Çí«ê’
	m_Distance = (*m_pPlayerPos - m_pos);
	m_Model.ChangeAnimation(eAnimationTrot);
	m_rot = Vector3D::Lerp(m_rot, Vector3D(0.0f, atan2f(m_Distance.x, m_Distance.z), 0.0), 0.5f);

	float leng = m_Distance.LengthSq();

	//çUåÇîªíf
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

void CEnemy::HitDamage()
{
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

	//èÛë‘ëJà⁄
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
	std::cout << "EnemyHit" << "\n";

	m_Hp--;
	m_FlinchNum++;
	if (m_Hp <= 0)
	{
		m_Model.SetTime(0);
		m_state = eState_Die;

		//ìGé©êgÇÃçUåÇÇÃìñÇΩÇËîªíËí‚é~
		int colliderNum = m_pCharaData->BoneCapsule.size();
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

CEnemy *CEnemyManager::Add(int type, Vector3D pos, Vector3D rot)
{
	CEnemy *e = new CEnemy(type, pos, rot);

	CCharacterManager::GetInstance()->Add(e);

	return e;
}