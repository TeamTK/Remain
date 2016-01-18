#include "StaticMeshCollider.h"
#include "ColliderManager.h"
#include "..\Mesh\StaticMesh.h"

StaticMesh_vs_LineSegmentCollider::StaticMesh_vs_LineSegmentCollider(StaticMesh *pStaticMeshHitInfo)
{
	m_pStaticMeshInfo = pStaticMeshHitInfo;
	pStaticMeshHitInfo->WorldMatrixBuilding();
	m_pNormal = new Vector3D[pStaticMeshHitInfo->GetFaceAllNum()];

	//モデルの変換行列
	Matrix world = *pStaticMeshHitInfo->GetWorldMatrix();
	Matrix local = *pStaticMeshHitInfo->GetLocalMatrix();
	Matrix m = local * world;

	Vector3D pos[3];

	//頂点データとマテリアルデータ
	const VertexInfo *ver = pStaticMeshHitInfo->GetVertex();
	const MaterialInfo *material = pStaticMeshHitInfo->GetMaterial();

	int cnt = 0;

	//マテリアルごとにポリゴンとの当たり判定をする
	int materialAllNum = pStaticMeshHitInfo->GetMaterialAllNum();
	for (int i = 0; i < materialAllNum; i++)
	{
		//マテリアル別のインデックスを取得
		const int *index = pStaticMeshHitInfo->GetPolygonIndex(i);
		int faceNum = material[i].dwNumFace;

		for (int j = 0; j < faceNum; j++)
		{
			pos[0] = ver[index[j * 3]].vPos;
			pos[1] = ver[index[j * 3 + 1]].vPos;
			pos[2] = ver[index[j * 3 + 2]].vPos;

			Vector3D e1 = pos[1] - pos[0];
			Vector3D e2 = pos[2] - pos[0];
			Vector3D normal = Vector3D::Cross(e2, e1).GetNormalize();

			m_pNormal[cnt] = normal;
			cnt++;
		}
	}

	ColliderManager::GetInstance()->Add(this);
}

StaticMesh_vs_LineSegmentCollider::~StaticMesh_vs_LineSegmentCollider()
{
	if (m_pNormal) delete[] m_pNormal;
	ColliderManager::GetInstance()->Clear(this);
}

StaticMesh_vs_SphereCollider::StaticMesh_vs_SphereCollider(StaticMesh *pStaticMeshHitInfo)
{
	m_pStaticMeshInfo = pStaticMeshHitInfo;
	pStaticMeshHitInfo->WorldMatrixBuilding();
	m_pNormal = new Vector3D[pStaticMeshHitInfo->GetFaceAllNum()];

	//モデルの変換行列
	Matrix world = *pStaticMeshHitInfo->GetWorldMatrix();
	Matrix local = *pStaticMeshHitInfo->GetLocalMatrix();
	Matrix m = local * world;

	Vector3D pos[3];

	//頂点データとマテリアルデータ
	const VertexInfo *ver = pStaticMeshHitInfo->GetVertex();
	const MaterialInfo *material = pStaticMeshHitInfo->GetMaterial();

	int cnt = 0;

	//マテリアルごとにポリゴンとの当たり判定をする
	int materialAllNum = pStaticMeshHitInfo->GetMaterialAllNum();
	for (int i = 0; i < materialAllNum; i++)
	{
		//マテリアル別のインデックスを取得
		const int *index = pStaticMeshHitInfo->GetPolygonIndex(i);
		int faceNum = material[i].dwNumFace;

		for (int j = 0; j < faceNum; j++)
		{
			 pos[0] = ver[index[j * 3]].vPos;
			 pos[1] = ver[index[j * 3 + 1]].vPos;
			 pos[2] = ver[index[j * 3 + 2]].vPos;

			 Vector3D e1 = pos[1] - pos[0];
			 Vector3D e2 = pos[2] - pos[0];
			 Vector3D normal = Vector3D::Cross(e2, e1).GetNormalize();

			 m_pNormal[cnt] = normal;
			 cnt++;
		}
	}

	ColliderManager::GetInstance()->Add(this);
}

StaticMesh_vs_SphereCollider::~StaticMesh_vs_SphereCollider()
{
	if (m_pNormal) delete[] m_pNormal;
	ColliderManager::GetInstance()->Clear(this);
}