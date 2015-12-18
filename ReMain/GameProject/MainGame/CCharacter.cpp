#include "CCharacter.h"
//#include "../GEKO//Figure/Sphere3D.h"
#include "..\GameSystem\Effect.h"

CCharacter::CCharacter(int type) : m_isActive(true)
{
	static SCharacterData _charaData[eCharacterMax] =
	{
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
			{
				SCharacterData::SBoneCalpule(0.4f,25,4),	//頭
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
	for (int i = 0; i < m_pCharaData->BoneCapsule.size(); i++)
	{
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

	m_BodyRadius = 1.0f;
	m_ColliderBody.Regist_S_vs_S(&m_pos, &m_BodyRadius, REGIST_FUNC(CCharacter::PushBody));
	m_ColliderBody.SetID(eHITID0, eHITID0);
}

CCharacter::~CCharacter()
{
	for (int i = 0; i < m_pCharaData->BoneCapsule.size(); i++)
	{
		m_pCollider[i].Release();
	}
	delete[] m_pCollider;
	delete[] m_pCapsule;
}

void CCharacter::Update()
{

	for (int i = 0; i < m_pCharaData->BoneCapsule.size(); i++)
	{
		m_pCapsule[i].radius = m_pCharaData->BoneCapsule[i].radius;
		m_pCapsule[i].segment.start = m_Model.GetBornPos(m_pCharaData->BoneCapsule[i].start);
		m_pCapsule[i].segment.end = m_Model.GetBornPos(m_pCharaData->BoneCapsule[i].end);

	}
	m_SphereMap.radius = m_pCharaData->collitionMapRad;
	m_SphereMap.pos = m_pos + Vector3D(0, m_SphereMap.radius, 0);
}

void CCharacter::Render()
{
	/*
	for (int i = 0; i < m_pCharaData->BoneCapsule.size(); i++)
	{
		Matrix matS, matTS, matTE;
		matS.Scale(m_pCapsule[i].radius, m_pCapsule[i].radius, m_pCapsule[i].radius);
		matTS.Transelate(m_pCapsule[i].segment.start.x, m_pCapsule[i].segment.start.y, m_pCapsule[i].segment.start.z);
		matTE.Transelate(m_pCapsule[i].segment.end.x, m_pCapsule[i].segment.end.y, m_pCapsule[i].segment.end.z);

		Fiqure::RenderLine3D(m_pCapsule[i].segment.start, m_pCapsule[i].segment.end, Vector3D(1.0f, 0.0f, 0.0f));
	}
	*/

	m_Model.SetTranselate(m_pos);
	m_Model.SetRotationRadian(m_rot.x, m_rot.y, m_rot.z);
	m_Model.Render();
	m_pos.y -= 0.01f;
}

void CCharacter::HitBullet()
{

}

void CCharacter::Capsule_vs_LineSegmentCallback(Result_Sphere& r)
{
	//血しぶきのエフェクト
	EffectInfo effectData;
	effectData.imageName = "Blood";
	effectData.num = 60;
	effectData.pos = r.position;
	effectData.scale = Vector3D(1.0f, 1.0f, 1.0f);
	effectData.speed = 0.1f;
	effectData.time = 120;
	EffectGeneration::Add(effectData);

	HitBullet();
}

void CCharacter::Sphere_vs_MeshCallback(Result_Porygon_Group& r)
{
	Vector3D v(0, 0, 0);
	for (int i = 0; i < r.hitNum; i++)
	{
		Result_Porygon *poly = &r.pArray[i];
		Vector3D nv = poly->normal * (m_SphereMap.radius - poly->dist);
		if (fabsf(v.x) < fabsf(nv.x)) v.x = nv.x;
		if (fabsf(v.y) < fabsf(nv.y)) v.y = nv.y;
		if (fabsf(v.z) < fabsf(nv.z)) v.z = nv.z;
	}
	m_pos += v;
}

void CCharacter::PushBody(Result_Sphere &data)
{
	Vector3D vec = m_pos - data.position;
	float l = (m_BodyRadius + data.radius) - vec.Length();

	m_pos += vec.GetNormalize() * (l * 0.1f);

	std::cout << l << "\n";
}

//******************************
//		CCharacterManager
//******************************
CCharacterManager *CCharacterManager::m_Obj = NULL;
CCharacterManager::CCharacterManager()
{
}

void CCharacterManager::Update()
{
	auto it = m_Core.begin();
	while (it != m_Core.end())
	{
		//生存していれば
		if ((*it)->m_isActive)
		{
			(*it)->Update();
			it++;
		}
		else {
			//削除処理
			delete *it;
			it = m_Core.erase(it);
		}
	}
}

void CCharacterManager::Render()
{
	auto it = m_Core.begin();
	while (it != m_Core.end())
	{
		(*it)->Render();
		it++;
	}
}

void CCharacterManager::Add(CCharacter* c)
{
	m_Core.push_back(c);
}

void CCharacterManager::AddPlayer(CCharacter *c)
{
	m_pPayer = c;
	Add(c);

}

void CCharacterManager::AllClear()
{
	auto it = m_Core.begin();
	for (; it != m_Core.end();)
	{
		delete *it;
		it = m_Core.erase(it);
	}
	m_Core.clear();
}