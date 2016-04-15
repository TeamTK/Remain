#include "DynamicMesh.h"
#include "..\..\Shader\ForwardShader\ForwardRendering.h"
#include "..\..\Shader\ShadowMap\ShaderShadowMap.h"
#include "..\..\Shader\ConstantShader.h"
#include "..\..\ImageSystem\Image.h"

DynamicMesh::DynamicMesh() :
	MeshBase(false, 0, 0, MeshState::eNothing),
	m_IsAnimUpdate(true),
	m_IsAnimEnd(false),
	m_AnimNum(0),
	m_AnimFrame(0.0f),
	m_pMeshData(nullptr)
{
	m_BoneMatrixFuncInfo.func = std::bind(&DynamicMesh::ModelBoneBuilding, this);
	WorldMatrixManager::GetInstance()->Add(&m_BoneMatrixFuncInfo);
}

DynamicMesh::~DynamicMesh()
{
	WorldMatrixManager::GetInstance()->Clear(&m_BoneMatrixFuncInfo);

	if (m_MeshState & MeshState::eBlockingLight)
	{
		ShaderShadowMap::GetInstance()->Clear(this);
	}

	ReleseCopyBoneTree(&m_Bone);
	m_pMeshData = nullptr;
	m_CopyBoneArray.clear();
	m_CopyBoneArray.shrink_to_fit();
}

DynamicMesh::DynamicMesh(const std::string &meshName, unsigned int priorityGroup, unsigned int priority, unsigned int meshState) :
	MeshBase(true, priorityGroup, priority, meshState),
	m_IsAnimUpdate(true),
	m_IsAnimEnd(false),
	m_pMeshData(nullptr)
{
	m_BoneMatrixFuncInfo.func = std::bind(&DynamicMesh::ModelBoneBuilding, this);
	WorldMatrixManager::GetInstance()->Add(&m_BoneMatrixFuncInfo);

	AllocationSkinMeshData(meshName);

	//モデルが光を遮って影になる対象
	if (meshState & MeshState::eBlockingLight)
	{
		ShaderShadowMap::GetInstance()->Add(this);
	}
}

void DynamicMesh::SetMeshState(unsigned int meshState)
{
	//事前に登録していたら解除
	if (m_MeshState & MeshState::eBlockingLight)
	{
		ShaderShadowMap::GetInstance()->Clear(this);
	}

	m_MeshState = meshState;

	//モデルが光を遮って影になる対象
	if (m_MeshState & MeshState::eBlockingLight)
	{
		assert(m_IsRenderingRegister && "レンダリングが登録されていないため「eBlockingLight」ができません");
		ShaderShadowMap::GetInstance()->Add(this);
	}
}

void DynamicMesh::SetAsset(const std::string &meshName)
{
	AllocationSkinMeshData(meshName);
}

void DynamicMesh::ChangeAnimation(unsigned int num)
{
	//アニメーションセットの範囲外アクセス防止
	const BoneInfo *pBoneData = m_pMeshData->GetBoneInfo();
	unsigned int AnimSetAllNum = pBoneData->AnimationSetFrameNum.size();
	if (num >= AnimSetAllNum) num = AnimSetAllNum - 1;
	else if (num <= 0) num = 0;

	m_AnimNum = num;
}

void DynamicMesh::SetPlayTime(float animSpeed)
{
	m_AnimFrame += animSpeed;
}

void DynamicMesh::SetTime(float animTime)
{
	m_AnimFrame = animTime;
}

void DynamicMesh::StartAnimation()
{
	m_IsAnimUpdate = true;
}

void DynamicMesh::StopAnimation()
{
	m_IsAnimUpdate = false;
}

const SkinVertexInfo *DynamicMesh::GetVertex() const
{
	return m_pMeshData->GetVertex();
}

float DynamicMesh::GetPlayTime() const
{
	return m_AnimFrame;
}

int DynamicMesh::GetPlayAnimation() const
{
	return m_AnimNum;
}

int DynamicMesh::GetFaceAllNum() const
{
	return m_pMeshData->GetMeshInfo()->faceNumAll;
}

int DynamicMesh::GetBoneNum(std::string name) const
{
	const BoneInfo *pBoneData = m_pMeshData->GetBoneInfo();
	unsigned int boneNum = pBoneData->BoneList.size();
	for (unsigned int i = 0; i < boneNum; i++)
	{
		if (pBoneData->BoneList[i]->boneName == name)
		{
			return i;
		}
	}
	return -1;
}

int DynamicMesh::GetBoneAllNum() const
{
	return m_pMeshData->GetBoneInfo()->BoneList.size();
}

std::string DynamicMesh::GetBoneName(int boneIndex) const
{
	const BoneInfo *pBoneData = m_pMeshData->GetBoneInfo();
	return pBoneData->BoneList[boneIndex]->boneName;
}

Matrix DynamicMesh::GetBoneMatrix(int boneIndex, bool isWorld) const
{
	if (isWorld) return m_CopyBoneArray[boneIndex]->parentAndChildMat * m_ModelMatrix;
	return m_CopyBoneArray[boneIndex]->parentAndChildMat;
}

Matrix DynamicMesh::GetBoneMatrix(std::string name, bool isWorld) const
{
	const BoneInfo *pBoneData = m_pMeshData->GetBoneInfo();

	int cnt = 0;
	for (auto& i : pBoneData->BoneList)
	{
		if (i->boneName == name)
		{
			if (isWorld) return m_CopyBoneArray[cnt]->parentAndChildMat * m_ModelMatrix;
			return m_CopyBoneArray[cnt]->parentAndChildMat;
		}
		cnt++;
	}

	return Matrix();
}

Vector3D DynamicMesh::GetBonePos(int boneIndex) const
{
	Matrix m = m_CopyBoneArray[boneIndex]->parentAndChildMat;
	return Vector3D(m._41, m._42, m._43) * m_ModelMatrix;
}

Vector3D DynamicMesh::GetBonePos(std::string name) const
{
	const BoneInfo *pBoneData = m_pMeshData->GetBoneInfo();

	int cnt = 0;
	for (auto& i : pBoneData->BoneList)
	{
		if (i->boneName == name)
		{
			Matrix m = m_CopyBoneArray[cnt]->parentAndChildMat;
			return Vector3D(m._41, m._42, m._43) * m_ModelMatrix;
		}
		cnt++;
	}

	return Vector3D();
}

void DynamicMesh::RenderOutline(float size = 1.0f)
{
	//RenderFunc(m_WorldMatrix);

	Matrix m = m_WorldMatrix;
	m._11 *= size;
	//m._22 *= size;
	m._33 *= size;

	Direct3D11::GetInstance()->SetRasterizer(D3D11_CULL_FRONT, D3D11_FILL_SOLID);
	//RenderFunc(m);
	Direct3D11::GetInstance()->SetRasterizer(D3D11_CULL_BACK, D3D11_FILL_SOLID);
}

void DynamicMesh::BoneDebug(eBoneDebug eBoneDebug) const
{
	int cnt = 0;
	for (auto& i : m_pMeshData->GetBoneInfo()->BoneList)
	{
		printf("%s\n", i->boneName.c_str());

		Matrix m = m_CopyBoneArray[cnt]->boneMat;
		printf("%f %f %f %f\n", m._11, m._12, m._13, m._14);
		printf("%f %f %f %f\n", m._21, m._22, m._23, m._24);
		printf("%f %f %f %f\n", m._31, m._32, m._33, m._34);
		printf("%f %f %f %f\n", m._41, m._42, m._43, m._44);

		cnt++;
	}
}

void DynamicMesh::AnimationDebug(int animNum) const
{

}

void DynamicMesh::ModelBoneBuilding()
{
	//アニメーション更新
	if (m_IsAnimUpdate)
	{
		m_pMeshData->Update(&m_Bone, m_AnimNum, &m_AnimFrame, &m_IsAnimEnd);
	}
}

void DynamicMesh::ForwardRendering()
{
	if (m_MeshState & eShadow)
	{
		RenderFunc(m_ModelMatrix, true);
	}
	else
	{
		RenderFunc(m_ModelMatrix, false);
	}
}

void DynamicMesh::DeferredRendering()
{

}

void DynamicMesh::AllocationSkinMeshData(const std::string &meshName)
{
	//初期化
	m_Diffuse.clear();
	m_Diffuse.shrink_to_fit();
	m_Specular.clear();
	m_Specular.shrink_to_fit();
	m_Ambient.clear();
	m_Ambient.shrink_to_fit();
	m_pMeshData = nullptr;

	m_pMeshData = DynamicMeshAsset::GetMesh(meshName);
	MeshInfo *data = m_pMeshData->GetMeshInfo();
	Vector4D diffuse;
	Vector3D specular;
	Vector3D ambient;

	//マテリアル割り当て
	for (int i = 0; i < data->materialNumAll; i++)
	{
		diffuse = data->pMaterial[i].diffuse;
		specular = data->pMaterial[i].specular;
		ambient = data->pMaterial[i].ambient;

		m_Diffuse.emplace_back(diffuse.x, diffuse.y, diffuse.z, diffuse.w);
		m_Specular.emplace_back(specular.x, specular.y, specular.z);
		m_Ambient.emplace_back(ambient.x, ambient.y, ambient.z);
	}
	const BoneInfo *pBoneData = m_pMeshData->GetBoneInfo();

	//ボーン初期化
	m_CopyBoneArray.clear();
	m_CopyBoneArray.shrink_to_fit();

	//ボーンをコピー
	m_CopyBoneArray.emplace_back(&m_Bone);
	ReleseCopyBoneTree(&m_Bone);
	m_pMeshData->CopyBoneTree(&m_Bone, &m_CopyBoneArray, pBoneData->bone.child);

	m_pLocalMatrix = &data->localMatrix;
}

void DynamicMesh::RenderFunc(Matrix &matrix, bool isShadow)
{
	ID3D11DeviceContext *pDeviceContext;
	pDeviceContext = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	const MeshInfo *data = m_pMeshData->GetMeshInfo();

	ForwardRendering::GetInstance()->SetDynamicMeshShader(pDeviceContext, m_MeshState);
	ConstantShader::GetInstance()->SetTransformMatrixConstantBuffer(pDeviceContext, matrix, isShadow);

	ConstantShader::GetInstance()->SetBoneConstantBuffer(pDeviceContext, GetBoneAllNum(), m_CopyBoneArray);

	//属性ごとにレンダリング

	//バーテックスバッファーをセット
	UINT Stride = sizeof(SkinVertexInfo);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &data->pVertexBuffer, &Stride, &offset);

	//マルチテクスチャ
	if (m_pImageInfo != nullptr)
	{
		pDeviceContext->PSSetShaderResources(2, 1, &m_pImageInfo->pTexture);
	}

	Vector4D specular;
	Vector4D ambient;

	//属性の数だけ、それぞれの属性のインデックスバッファ－を描画
	for (int i = 0; i < data->materialNumAll; i++)
	{
		//使用されていないマテリアル対策
		if (data->pMaterial[i].numPolygon == 0) continue;

		//インデックスバッファーをセット
		pDeviceContext->IASetIndexBuffer(data->ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);

		specular.x = m_Specular[i].x;
		specular.y = m_Specular[i].y;
		specular.z = m_Specular[i].z;
		specular.w = m_Diffuse[i].w;

		ambient.x = m_Ambient[i].x;
		ambient.y = m_Ambient[i].y;
		ambient.z = m_Ambient[i].z;
		ambient.w = m_Diffuse[i].w;

		ConstantShader::GetInstance()->SetMaterialConstantBuffer(pDeviceContext, m_Diffuse[i], specular, ambient);

		pDeviceContext->PSSetSamplers(0, 1, &data->pSampleLinear);
		pDeviceContext->PSSetShaderResources(0, 1, &data->pMaterial[i].pTexture);

		//プリミティブをレンダリング
		pDeviceContext->DrawIndexed(data->pMaterial[i].numPolygon * 3, 0, 0);
	}

	//テクスチャリソース初期化
	ID3D11ShaderResourceView* pShaderResource = nullptr;
	pDeviceContext->PSSetShaderResources(0, 1, &pShaderResource);
	pDeviceContext->PSSetShaderResources(1, 1, &pShaderResource);
	pDeviceContext->PSSetShaderResources(2, 1, &pShaderResource);
}

void DynamicMesh::ReleseCopyBoneTree(CopyBone *pBoneCopy) const
{
	//子ボーン
	if (pBoneCopy->child != nullptr)
	{
		ReleseCopyBoneTree(pBoneCopy->child);
		delete pBoneCopy->child;
	}

	//親ボーン
	if (pBoneCopy->brother != nullptr)
	{
		ReleseCopyBoneTree(pBoneCopy->brother);
		delete pBoneCopy->brother;
	}
}