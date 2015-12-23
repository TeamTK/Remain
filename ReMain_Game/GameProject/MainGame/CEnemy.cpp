#include "CEnemy.h"

CEnemy::CEnemy(Vector3D pos, Vector3D rot) :
	Character(10, "Enemy", 1),
	m_isChase(false),
	m_FlinchNum(0),
	m_state(eState_Idle)
{
	m_SphereMap.radius = 0.2f;

	//各部位のカプセルの情報
	BoneCapsule.emplace_back(0.4f, 25, 4);	  //頭
	BoneCapsule.emplace_back(0.4f, 2, 4);   //胴体
	BoneCapsule.emplace_back(0.4f, 6, 8);   //左腕
	BoneCapsule.emplace_back(0.4f, 16, 18); //右腕
	BoneCapsule.emplace_back(0.4f, 26, 28); //左足
	BoneCapsule.emplace_back(0.4f, 30, 32); //右足

	m_Model.SetAsset("Monster_A");

	//球（弾）との判定用
	m_pCollider = new Collider[BoneCapsule.size()];
	m_pCapsule = new CapsuleInfo[BoneCapsule.size()];

	unsigned int bornNum = BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++)
	{
		m_pCollider[i].Regist_C_vs_S(&m_pCapsule[i].segment.start, &m_pCapsule[i].segment.end, &m_pCapsule[i].radius, REGIST_FUNC(Character::Capsule_vs_LineSegmentCallback));
		//eHITID0…マップ
		//eHITID1…プレイヤー
		//eHITID2…敵
		//eHITID3…弾
		m_pCollider[i].SetID(eHITID2, eHITID3);
	}

	m_pos = pos;
	m_rot = rot;

	//視界システム
	m_SightData.angle = 60.0f;
	m_SightData.distance = 10.0f;
	m_SightData.pSightPos = &m_pos;
	m_SightData.pSightVec = &m_SightVec;
	m_Sight.Regist(&m_SightData, REGIST_FUNC(CEnemy::HitSight));

	m_BodyRadius = 0.5f; //敵の体の半径
}

CEnemy::~CEnemy()
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
	//プレイヤーを追跡
	m_Distance = (*m_pPlayerPos - m_pos);
	m_Model.ChangeAnimation(eAnimationTrot);
	m_rot = Vector3D::Lerp(m_rot, Vector3D(0.0f, atan2f(m_Distance.x, m_Distance.z), 0.0), 0.5f);

	float leng = m_Distance.LengthSq();

	//攻撃判断
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
	if (m_Model.GetPlayTime() == 29) Task::SetKill();
}

void CEnemy::Update()
{
	//攻撃される側の当たり判定更新
	unsigned int bornNum = BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++)
	{
		m_pCapsule[i].radius = BoneCapsule[i].radius;
		m_pCapsule[i].segment.start = m_Model.GetBornPos(BoneCapsule[i].start);
		m_pCapsule[i].segment.end = m_Model.GetBornPos(BoneCapsule[i].end);

	}
	//m_SphereMap.radius = m_pCharaData->collitionMapRad;
	m_SphereMap.pos = m_pos + Vector3D(0, m_SphereMap.radius, 0);

	m_SightVec = m_Model.GetAxisZ(1.0f);
	m_Model.SetPlayTime(30);

	//状態遷移
	switch (m_state)
	{
	case eState_Attack:
		Attack();
		//std::cout << "ATTACK" << "\n";
		break;

	case eState_Idle:
		Idle();
		//std::cout << "IDLE" << "\n";
		break;

	case eState_Chase:
		Chase();
		//std::cout << "CHASE" << "\n";
		break;

	case eState_HitDamage:
		HitDamage();
		//std::cout << "HIT_DAMEGE" << "\n";
		break;

	case eState_Die:
		//std::cout << "DIE" << "\n";
		Die();

	default:
		break;
	}
	Character::Update();
}

void CEnemy::HitBullet()
{
	m_Hp--;
	m_FlinchNum++;
	if (m_Hp <= 0)
	{
		m_Model.SetTime(0);
		m_state = eState_Die;

		//敵自身の攻撃の当たり判定停止
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