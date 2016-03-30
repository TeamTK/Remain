#include "WorldMatrixManager.h"
#include <list>

//Pimplイディオムでリスト保持
class WorldMatrixManager::ListPimpl
{
public:
	std::list<WorldMatrixInfo*> m_List;
};

//ワールド行列管理クラス
WorldMatrixManager::WorldMatrixManager()
{
	m_pListPimpl = new ListPimpl;
}

WorldMatrixManager::~WorldMatrixManager()
{
	m_pListPimpl->m_List.clear();
	delete m_pListPimpl;
}

WorldMatrixManager *WorldMatrixManager::GetInstance()
{
	static WorldMatrixManager WorldMatrixManager;
	return &WorldMatrixManager;
}

void WorldMatrixManager::Add(WorldMatrixInfo *worldMatrixInfo)
{
	m_pListPimpl->m_List.push_front(worldMatrixInfo);
}

void WorldMatrixManager::Update()
{
	D3DXQUATERNION qOut(0, 0, 0, 1); //単位クォータニオン
	D3DXQUATERNION qX(0, 0, 0, 1); //単位クォータニオン
	D3DXQUATERNION qY(0, 0, 0, 1); //単位クォータニオン
	D3DXQUATERNION qZ(0, 0, 0, 1); //単位クォータニオン
	Vector3D xAxis(1, 0, 0); //Xの中心軸
	Vector3D yAxis(0, 1, 0); //Yの中心軸
	Vector3D zAxis(0, 0, 1); //Zの中心軸
	Matrix mat;

	for (auto& i : m_pListPimpl->m_List)
	{
		D3DXQuaternionRotationAxis(&qX, &xAxis, i->pRotation->x);
		D3DXQuaternionRotationAxis(&qY, &yAxis, i->pRotation->y);
		D3DXQuaternionRotationAxis(&qZ, &zAxis, i->pRotation->z);
		qOut = qX * qY * qZ;

		//クオータニオンから行列に変更
		D3DXMatrixRotationQuaternion(&mat, &qOut);

		//拡大縮小
		mat._11 *= i->pScale->x;
		mat._21 *= i->pScale->x;
		mat._31 *= i->pScale->x;

		mat._12 *= i->pScale->y;
		mat._22 *= i->pScale->y;
		mat._32 *= i->pScale->y;

		mat._13 *= i->pScale->z;
		mat._23 *= i->pScale->z;
		mat._33 *= i->pScale->z;

		//平行移動
		mat._41 = i->pTranselate->x;
		mat._42 = i->pTranselate->y;
		mat._43 = i->pTranselate->z;

		*i->pWorldMatrix = mat;
		*i->pModelMatrix = *i->pLocalMatrix * *i->pWorldMatrix;
	}
}

void WorldMatrixManager::Clear()
{
	auto it = m_pListPimpl->m_List.begin();
	for (; it != m_pListPimpl->m_List.end(); it++)
	{
		//ワールド行列がなかったらリストから除外
		if ((*it)->pWorldMatrix == nullptr)
		{
			it = m_pListPimpl->m_List.erase(it);
			break;
		}
	}
}

void WorldMatrixManager::AllClear()
{
	m_pListPimpl->m_List.clear();
}