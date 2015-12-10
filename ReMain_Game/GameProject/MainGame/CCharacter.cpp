#include "CCharacter.h"
#include "../GEKO//Figure/Sphere3D.h"
CCharacter::CCharacter(int type) :m_isActive(true){
	static SCharacterData _charaData[eCharacterMax] = {
		{
			"Player",
			100,
			0.2f,
			{},
		},
		{
			"Monster_A",
			10,
			0.2f,
			{	SCharacterData::SBoneCalpule(0.3f,25,25),	//頭
				SCharacterData::SBoneCalpule(0.4f,2,4),		//胴体
				SCharacterData::SBoneCalpule(0.2f,6,8),		//左腕
				SCharacterData::SBoneCalpule(0.2f,16,18) ,	//右腕
				SCharacterData::SBoneCalpule(0.2f,26,28) ,	//左足
				SCharacterData::SBoneCalpule(0.2f,30,32) ,	//右足
			},
		},
	};
	m_pCharaData = &_charaData[type];
	m_Model.SetAsset(m_pCharaData->aseetName);
	//球（弾）との判定用
	m_pCollider = new Collider[m_pCharaData->BoneCapsule.size()];
	m_pCapsule = new CapsuleInfo[m_pCharaData->BoneCapsule.size()];
	for (int i = 0; i < m_pCharaData->BoneCapsule.size(); i++) {
		m_pCollider[i].Regist_C_vs_S(&m_pCapsule[i].segment.start, &m_pCapsule[i].segment.end, &m_pCapsule[i].radius, REGIST_FUNC(CCharacter::Capsule_vs_LineSegmentCallback));
		//eHITID0…マップ
		//eHITID1…プレイヤー
		//eHITID2…敵
		//eHITID3…弾
		m_pCollider[i].SetID(eHITID2, eHITID3);
	}
	//マップとの判定用
	m_ColliderMap.Regist_S_vs_SMesh(&m_SphereMap.pos, &m_SphereMap.radius, REGIST_FUNC(CCharacter::Sphere_vs_MeshCallback));
	m_ColliderMap.SetID(eHITID1 | eHITID2, eHITID0);
	m_Hp = m_pCharaData->hp;

}
CCharacter::~CCharacter() {
	for (int i = 0; i < m_pCharaData->BoneCapsule.size(); i++) {
		m_pCollider[i].Release();
	}
	delete[] m_pCollider;
	delete[] m_pCapsule;
}
void CCharacter::Update() {

	for (int i = 0; i < m_pCharaData->BoneCapsule.size(); i++) {
		m_pCapsule[i].radius = m_pCharaData->BoneCapsule[i].radius;
		m_pCapsule[i].segment.start = m_Model.GetBornPos(m_pCharaData->BoneCapsule[i].start);
		m_pCapsule[i].segment.end = m_Model.GetBornPos(m_pCharaData->BoneCapsule[i].end);
	}
	m_SphereMap.radius = m_pCharaData->collitionMapRad;
	m_SphereMap.pos = m_pos + Vector3D(0, m_SphereMap.radius, 0);
}
void CCharacter::Render() {
	static Sphere3D *debug_model=NULL;
	if (!debug_model) {
		debug_model = new Sphere3D;
	} else {
		for (int i = 0; i < m_pCharaData->BoneCapsule.size(); i++) {
			Matrix matS, matTS, matTE;
			matS.Scale(m_pCapsule[i].radius, m_pCapsule[i].radius, m_pCapsule[i].radius);
			matTS.Transelate(m_pCapsule[i].segment.start.x, m_pCapsule[i].segment.start.y, m_pCapsule[i].segment.start.z);
			matTE.Transelate(m_pCapsule[i].segment.end.x, m_pCapsule[i].segment.end.y, m_pCapsule[i].segment.end.z);
			debug_model->Render(matS*matTS,Vector3D(1,0,0));
			debug_model->Render(matS*matTE, Vector3D(1, 0, 0));
		}
	}
	m_Model.SetTranselate(m_pos);
	m_Model.SetRotationRadian(m_rot.x, m_rot.y, m_rot.z);
	m_Model.Render();
}

void CCharacter::HitBullet() {

}
//void CCharacter::Capsule_vs_LineSegmentCallback(Result_LineSegment& r) {
void CCharacter::Capsule_vs_LineSegmentCallback(Result_Sphere& r) {
	HitBullet();
}
void CCharacter::Sphere_vs_MeshCallback(Result_Porygon_Group& r) {
	Vector3D v(0,0,0);
	for (int i = 0; i < r.hitNum; i++) {
		Result_Porygon *poly = &r.pArray[i];
		Vector3D nv = poly->normal * (m_SphereMap.radius - poly->dist);
		if (fabsf(v.x) < fabsf(nv.x)) v.x = nv.x;
		if (fabsf(v.y) < fabsf(nv.y)) v.y = nv.y;
		if (fabsf(v.z) < fabsf(nv.z)) v.z = nv.z;
	}
	m_pos += v;
}
CCharacterManager *CCharacterManager::m_Obj = NULL;
CCharacterManager::CCharacterManager() {
}

void CCharacterManager::LoadFile(const char* filepath) {
	/*
	仮
	*/
	DynamicMeshAsset::LoadMesh("media\\Player.x", "Player");
	DynamicMeshAsset::LoadMesh("media\\Monster_A.x", "Monster_A");

}
void CCharacterManager::Update() {
	auto it = m_Core.begin();
	while (it != m_Core.end()) {
		//生存していれば
		if ((*it)->m_isActive) {
			(*it)->Update();
			it++;
		} else {
			//削除処理
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