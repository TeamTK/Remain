#include "StaticMeshCollider.h"
#include "ColliderManager.h"
#include "..\Mesh\StaticMesh\StaticMesh.h"

StaticMesh_vs_LineSegmentCollider::StaticMesh_vs_LineSegmentCollider(StaticMesh *pStaticMeshHitInfo, bool isGPU) :
	m_pNormal(nullptr),
	m_isGPU(false)
{
	m_pStaticMeshInfo = pStaticMeshHitInfo;
	pStaticMeshHitInfo->SetModelMatrixBuilding();

	//当たり判定に使う法線
	if (m_pNormal != nullptr) delete[] m_pNormal;
	m_pNormal = new Vector3D[pStaticMeshHitInfo->GetFaceAllNum()];

	Vector3D pos[3];

	//頂点データとポリゴンのインデックス
	const VertexInfo *ver = pStaticMeshHitInfo->GetVertex();
	const IndexInfo *index = pStaticMeshHitInfo->GetIndex();
	int polyNum = pStaticMeshHitInfo->GetFaceAllNum();

	//全ての三角形の法線取得
	for (int i = 0; i < polyNum; i++)
	{
		pos[0] = ver[index[i].vertexIndex[0]].pos;
		pos[1] = ver[index[i].vertexIndex[1]].pos;
		pos[2] = ver[index[i].vertexIndex[2]].pos;

		m_pNormal[i] = Vector3D::Cross(pos[2] - pos[0], pos[1] - pos[0]).GetNormalize();
	}

	m_isGPU = isGPU;
	if (m_isGPU) m_Copmute.Init(*pStaticMeshHitInfo, m_pNormal);
	
	ColliderManager::GetInstance()->Add(this);
}

StaticMesh_vs_LineSegmentCollider::~StaticMesh_vs_LineSegmentCollider()
{
	if (m_pNormal != nullptr) delete[] m_pNormal;
	m_Copmute.Release();
	ColliderManager::GetInstance()->Clear(this);
}

StaticMesh_vs_SphereCollider::StaticMesh_vs_SphereCollider(StaticMesh *pStaticMeshHitInfo) :
	m_pNormal(nullptr)
{
	m_pStaticMeshInfo = pStaticMeshHitInfo;
	pStaticMeshHitInfo->SetModelMatrixBuilding();

	//当たり判定に使う法線
	if (m_pNormal != nullptr) delete[] m_pNormal;
	m_pNormal = new Vector3D[pStaticMeshHitInfo->GetFaceAllNum()];

	Vector3D pos[3];

	//頂点データとポリゴンのインデックス
	const VertexInfo *ver = pStaticMeshHitInfo->GetVertex();
	const IndexInfo *index = pStaticMeshHitInfo->GetIndex();
	int polyNum = pStaticMeshHitInfo->GetFaceAllNum();

	//全ての三角形の法線取得
	for (int i = 0; i < polyNum; i++)
	{
		pos[0] = ver[index[i].vertexIndex[0]].pos;
		pos[1] = ver[index[i].vertexIndex[1]].pos;
		pos[2] = ver[index[i].vertexIndex[2]].pos;

		m_pNormal[i] = Vector3D::Cross(pos[2] - pos[0], pos[1] - pos[0]).GetNormalize();
	}

	ColliderManager::GetInstance()->Add(this);
}

StaticMesh_vs_SphereCollider::~StaticMesh_vs_SphereCollider()
{
	if (m_pNormal != nullptr) delete[] m_pNormal;
	ColliderManager::GetInstance()->Clear(this);
}