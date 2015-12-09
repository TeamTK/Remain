#include "CEnemy.h"

CEnemy::CEnemy() {
	m_Model.SetAsset("Monster_A");
}
CEnemy::CEnemy(Vector3D pos) : CEnemy(){
	m_pos = pos;
}
void CEnemy::Update() {

	m_Model.ChangeAnimation(0);
	m_Model.SetPlayTime(30);
	CCharacter::Update();

}

CEnemy *CEnemyManager::Add(Vector3D pos) {
	CEnemy *e = new CEnemy(pos);

	CCharacterManager::GetInstance()->Add(e);

	return e;
}