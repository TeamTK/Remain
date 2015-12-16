#include "CEnemy.h"

CEnemy::CEnemy(int type,Vector3D pos) : CCharacter(type){
	m_pos = pos;
	m_SightData.angle = 60.0f;
	m_SightData.distance = 5.0f;
	m_SightData.pSightPos = &m_pos;
	m_SightData.pSightVec = &m_SightVec;
	m_Sight.Regist(&m_SightData, REGIST_FUNC(CEnemy::HitSight));
}
void CEnemy::Hit() {
	m_Model.ChangeAnimation(eAnimationHit);
	m_Model.SetPlayTime(30);
	if (m_cnt++>60) {
		m_state = eState_Idle;
	}
}
void CEnemy::Idle() {
	m_Model.ChangeAnimation(eAnimationIdle);
	m_Model.SetPlayTime(30);

}
void CEnemy::Die() {
	m_Model.ChangeAnimation(eAnimationDie);
	m_Model.SetPlayTime(30);
	if (m_cnt++>60) {
		m_isActive = false;

	}
}
void CEnemy::Update() {
	m_SightVec = m_Model.GetAxisZ(1.0f);

	switch (m_state)
	{
	case eState_Idle:
		Idle();
		break;

	case eState_Chase:
		m_pos += (*m_pPlayerPos - m_pos).GetNormalize() * 0.1f;

		break;

	case eState_Hit:
		Hit();
		break;
	case eState_Die:
		Die();
	default:
		break;
	}
	CCharacter::Update();
}

void CEnemy::HitBullet() {
	m_cnt = 0;
	m_Hp--;
	if(m_Hp<=0) {
		m_state = eState_Die;
	} else {
		m_state = eState_Hit;
	}

}

void CEnemy::HitSight(const Vector3D *pPos)
{
	m_pPlayerPos = pPos;
	//m_state = eState_Chase;
	m_Sight.Sleep();
}

CEnemy *CEnemyManager::Add(int type,Vector3D pos) {
	CEnemy *e = new CEnemy(type,pos);

	CCharacterManager::GetInstance()->Add(e);

	return e;
}