#include "Monster_A.h"

Monster_A::Monster_A(Vector3D pos, Vector3D rot) :
	Enemy(pos, rot, "Monster_A")
{
	//各部位のカプセルの情報
	m_BoneCapsule.emplace_back(0.4f, 25, 4);	//頭
	m_BoneCapsule.emplace_back(0.6f, 2, 4);   //胴体
	m_BoneCapsule.emplace_back(0.5f, 6, 8);   //左腕
	m_BoneCapsule.emplace_back(0.5f, 16, 18); //右腕
	m_BoneCapsule.emplace_back(0.5f, 26, 28); //左足
	m_BoneCapsule.emplace_back(0.5f, 30, 32); //右足

	//球（弾）との判定用
	m_pCollider = new Collider[m_BoneCapsule.size()];
	m_pCapsule = new CapsuleInfo[m_BoneCapsule.size()];

	//攻撃の判定
	m_pHitAttack = new Collider[m_BoneCapsule.size()];

	unsigned int bornNum = m_BoneCapsule.size();
	for (unsigned int i = 0; i < bornNum; i++)
	{
		m_pCollider[i].Regist_C_vs_S(&m_pCapsule[i].segment.start, &m_pCapsule[i].segment.end,
			&m_pCapsule[i].radius, REGIST_FUNC(Enemy::HitBullet));
		m_pCollider[i].SetID(eHITID0, eHITID1);

		m_pHitAttack[i].Regist_C_vs_C(&m_pCapsule[i].segment.start, &m_pCapsule[i].segment.end,
			&m_pCapsule[i].radius, REGIST_FUNC(Enemy::HitAttack));
		m_pHitAttack[i].SetID(eHITID1, eHITID0);
		m_pHitAttack[i].Sleep();
	}
}

Monster_A::~Monster_A()
{
}

void Monster_A::Update()
{
	Enemy::Update();
}