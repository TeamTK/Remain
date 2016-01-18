#include "StaticMeshCollider.h"
#include "ColliderManager.h"
#include "..\Mesh\StaticMesh.h"

StaticMesh_vs_LineSegmentCollider::StaticMesh_vs_LineSegmentCollider(StaticMesh *pStaticMeshHitInfo)
{
	m_pStaticMeshInfo = pStaticMeshHitInfo;
	pStaticMeshHitInfo->WorldMatrixBuilding();
	m_pNormal = new Vector3D[pStaticMeshHitInfo->GetFaceAllNum()];

	//���f���̕ϊ��s��
	Matrix world = *pStaticMeshHitInfo->GetWorldMatrix();
	Matrix local = *pStaticMeshHitInfo->GetLocalMatrix();
	Matrix m = local * world;

	Vector3D pos[3];

	//���_�f�[�^�ƃ}�e���A���f�[�^
	const VertexInfo *ver = pStaticMeshHitInfo->GetVertex();
	const MaterialInfo *material = pStaticMeshHitInfo->GetMaterial();

	int cnt = 0;

	//�}�e���A�����ƂɃ|���S���Ƃ̓����蔻�������
	int materialAllNum = pStaticMeshHitInfo->GetMaterialAllNum();
	for (int i = 0; i < materialAllNum; i++)
	{
		//�}�e���A���ʂ̃C���f�b�N�X���擾
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

	//���f���̕ϊ��s��
	Matrix world = *pStaticMeshHitInfo->GetWorldMatrix();
	Matrix local = *pStaticMeshHitInfo->GetLocalMatrix();
	Matrix m = local * world;

	Vector3D pos[3];

	//���_�f�[�^�ƃ}�e���A���f�[�^
	const VertexInfo *ver = pStaticMeshHitInfo->GetVertex();
	const MaterialInfo *material = pStaticMeshHitInfo->GetMaterial();

	int cnt = 0;

	//�}�e���A�����ƂɃ|���S���Ƃ̓����蔻�������
	int materialAllNum = pStaticMeshHitInfo->GetMaterialAllNum();
	for (int i = 0; i < materialAllNum; i++)
	{
		//�}�e���A���ʂ̃C���f�b�N�X���擾
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