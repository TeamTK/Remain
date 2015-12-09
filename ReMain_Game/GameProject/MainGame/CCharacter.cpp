#include "CCharacter.h"

CCharacter::CCharacter() :m_isActive(true), m_Capsule(Vector3D(0,0,0),Vector3D(0,0,0),0.0){
//m_CapsuleCollider(&m_Capsule.segment.start, &m_Capsule.segment.end, &m_Capsule.radius, std::bind(&CCharacter::Capsule_vs_LineSegmentCallback, std::ref(*this), std::placeholders::_1)){
	//m_CapsuleCollider.SetID(eHITID2, eHITID0 | eHITID1|eHITID2);
	m_Hp = 100;
//	ColliderManager::GetInstance()->Add(&m_CapsuleCollider);
}
void CCharacter::Update() {
	m_Capsule.radius = 5.0f;
	m_Capsule.segment.start = m_pos + Vector3D(0, 5.0f + m_Capsule.radius, 0);
	m_Capsule.segment.end = m_pos + Vector3D(0,m_Capsule.radius, 0);
	m_Model.SetTranselate(m_pos);
	m_Model.SetRotationRadian(m_rot.x, m_rot.y, m_rot.z);
}
void CCharacter::Render() {
	m_Model.Render();
}
/*
void CCharacter::Capsule_vs_LineSegmentCallback(Result_LineSegment& r) {
	m_Hp -= 1;
}
*/
CCharacterManager *CCharacterManager::m_Obj = NULL;
CCharacterManager::CCharacterManager() {
}

void CCharacterManager::LoadFile(const char* filepath) {
	/*
	‰¼
	*/
	//DynamicMeshAsset::LoadMesh("media\\Player.x", "Player");
	DynamicMeshAsset::LoadMesh("media\\Monster_A.x", "Monster_A");

}
void CCharacterManager::Update() {
	auto it = m_Core.begin();
	while (it != m_Core.end()) {
		//¶‘¶‚µ‚Ä‚¢‚ê‚Î
		if ((*it)->m_isActive) {
			(*it)->Update();
			it++;
		} else {
			//íœˆ—
			delete *it;
			it = m_Core.erase(it);
		}
	}
}
void CCharacterManager::Render() {
	auto it = m_Core.begin();
	while (it != m_Core.end()) {
		(*it)->Render();
		it++;
	}
}
void CCharacterManager::Add(CCharacter* c) {
	m_Core.push_back(c);
}
void CCharacterManager::AddPlayer(CCharacter *c) {
	m_pPayer = c;
	Add(c);

}