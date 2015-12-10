#include "CEnemy.h"

CEnemy::CEnemy(int type,Vector3D pos) : CCharacter(type){
	m_pos = pos;
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
	switch (m_state)
	{
	case eState_Idle:
		Idle();
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
CEnemy *CEnemyManager::Add(int type,Vector3D pos) {
	CEnemy *e = new CEnemy(type,pos);

	CCharacterManager::GetInstance()->Add(e);

	return e;
}