#include "DynamicMesh.h"
#include "..\..\Shader\DynamicMeshShader\DynamicMeshShader.h"
#include "..\..\Shader\ShadowMap\ShaderShadowMap.h"
#include "..\..\Shader\ConstantShader.h"
#include "..\..\ImageSystem\Image.h"
#include <assert.h>

DynamicMesh::DynamicMesh() :
	m_IsAnimUpdate(true),
	m_IsAnimEnd(false),
	m_AnimNum(0),
	m_AnimFrame(0.0f),
	m_pMeshData(nullptr)
{
}

DynamicMesh::~DynamicMesh()
{
	ShaderShadowMap::GetInstance()->Clear(this);
	ReleseCopyBornTree(&m_Born);
	m_pMeshData = nullptr;
	m_CopyBornArray.clear();
	m_CopyBornArray.shrink_to_fit();
}

DynamicMesh::DynamicMesh(const std::string &meshName, bool isLightInterrupted) :
	m_IsAnimUpdate(true),
	m_IsAnimEnd(false),
	m_pMeshData(nullptr)
{
	AllocationSkinMeshData(meshName);
	if (isLightInterrupted)
	{
		ShaderShadowMap::GetInstance()->Clear(this);
		ShaderShadowMap::GetInstance()->Add(this);
	}
}

void DynamicMesh::SetAsset(const std::string &meshName, bool isLightInterrupted)
{
	AllocationSkinMeshData(meshName);
	if (isLightInterrupted)
	{
		ShaderShadowMap::GetInstance()->Clear(this);
		ShaderShadowMap::GetInstance()->Add(this);
	}
}

void DynamicMesh::ChangeAnimation(unsigned int num)
{	
	//アニメーションセットの範囲外アクセス防止
	const BornInfo *pBornData = m_pMeshData->GetBornInfo();
	unsigned int AnimSetAllNum = pBornData->AnimationSetFrameNum.size();
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

int DynamicMesh::GetBornNum(std::string name) const
{
	const BornInfo *pBornData = m_pMeshData->GetBornInfo();
	unsigned int bornNum = pBornData->BornList.size();
	for (unsigned int i = 0; i < bornNum; i++)
	{
		if (pBornData->BornList[i]->BornName == name)
		{
			return i;
		}
	}
	return -1;
}

int DynamicMesh::GetBornAllNum() const
{
	return m_pMeshData->GetBornInfo()->BornList.size();
}

std::string DynamicMesh::GetBornName(int bornIndex) const
{
	const BornInfo *pBornData = m_pMeshData->GetBornInfo();
	return pBornData->BornList[bornIndex]->BornName;
}

Matrix DynamicMesh::GetBornMatrix(int bornIndex, bool isWorld) const
{
	if (isWorld) return m_CopyBornArray[bornIndex]->ParentAndChildMat * m_SynthesisMatrix;
	return m_CopyBornArray[bornIndex]->ParentAndChildMat;
}

Matrix DynamicMesh::GetBornMatrix(std::string name, bool isWorld) const
{
	const BornInfo *pBornData = m_pMeshData->GetBornInfo();

	int cnt = 0;
	for (auto& i : pBornData->BornList)
	{
		if (i->BornName == name)
		{
			if (isWorld) return m_CopyBornArray[cnt]->ParentAndChildMat * m_SynthesisMatrix;
			return m_CopyBornArray[cnt]->ParentAndChildMat;
		}
		cnt++;
	}

	return Matrix();
}

Vector3D DynamicMesh::GetBornPos(int bornIndex) const
{
	Matrix m = m_CopyBornArray[bornIndex]->ParentAndChildMat;
	return Vector3D(m._41, m._42, m._43) * m_SynthesisMatrix;
}

Vector3D DynamicMesh::GetBornPos(std::string name) const
{
	const BornInfo *pBornData = m_pMeshData->GetBornInfo();

	int cnt = 0;
	for (auto& i : pBornData->BornList)
	{
		if (i->BornName == name)
		{
			Matrix m = m_CopyBornArray[cnt]->ParentAndChildMat;
			return Vector3D(m._41, m._42, m._43) * m_SynthesisMatrix;
		}
		cnt++;
	}

	return Vector3D();
}

void DynamicMesh::Render()
{
	RenderFunc(m_SynthesisMatrix);
}

void DynamicMesh::RenderOutline(float size = 1.0f)
{
	RenderFunc(m_WorldMatrix);

	Matrix m = m_WorldMatrix;
	m._11 *= size;
	//m._22 *= size;
	m._33 *= size;

	Direct3D11::GetInstance()->SetRasterizer(D3D11_CULL_FRONT, D3D11_FILL_SOLID);
	RenderFunc(m);
	Direct3D11::GetInstance()->SetRasterizer(D3D11_CULL_BACK, D3D11_FILL_SOLID);
}

void DynamicMesh::RenderMatrix(Matrix &matrix)
{
	RenderFunc(m_SynthesisMatrix * matrix);
}

void DynamicMesh::BornDebug(eBornDebug eBornDebug) const
{
	int cnt = 0;
	for (auto& i : m_pMeshData->GetBornInfo()->BornList)
	{
		printf("%s\n", i->BornName.c_str());

		Matrix m = m_CopyBornArray[cnt]->bornMat;
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
	const BornInfo *pBornData = m_pMeshData->GetBornInfo();

	//ボーン初期化
	m_CopyBornArray.clear();
	m_CopyBornArray.shrink_to_fit();

	//ボーンをコピー
	m_CopyBornArray.emplace_back(&m_Born);
	ReleseCopyBornTree(&m_Born);
	m_pMeshData->CopyBornTree(&m_Born, &m_CopyBornArray, pBornData->sBorn.child);

	m_WorldMatrixInfo.pLocalMatrix = &data->localMatrix;
}

void DynamicMesh::RenderFunc(Matrix &matrix)
{
	ID3D11DeviceContext *pDeviceContext;
	pDeviceContext = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	assert(m_pMeshData != nullptr && "メッシュ情報がありません");

	const MeshInfo *data = m_pMeshData->GetMeshInfo();

	DynamicMeshShader::GetInstance()->SetShader(pDeviceContext, data->isTexture, false);

	ConstantShader::GetInstance()->SetCommonInfoConstantBuffer();
	ConstantShader::GetInstance()->SetTransformMatrixConstantBuffer(pDeviceContext, matrix, false);

	//アニメーション更新
	if (m_IsAnimUpdate) 
	{
		m_pMeshData->Update(&m_Born, m_AnimNum, &m_AnimFrame, &m_IsAnimEnd);
	}
	ConstantShader::GetInstance()->SetBornConstantBuffer(pDeviceContext, GetBornAllNum(), m_CopyBornArray);

	//属性ごとにレンダリング

	//バーテックスバッファーをセット
	UINT Stride = sizeof(SkinVertexInfo);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &data->pVertexBuffer, &Stride, &offset);

	//マルチテクスチャ
	if (m_pImage != nullptr)
	{
		pDeviceContext->PSSetShaderResources(2, 1, &m_pImage->m_pImageData->GetImageInfo()->pTexture);
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

void DynamicMesh::ReleseCopyBornTree(CopyBorn *pBornCopy) const
{
	//子ボーン
	if (pBornCopy->child != nullptr)
	{
		ReleseCopyBornTree(pBornCopy->child);
		delete pBornCopy->child;
	}

	//親ボーン
	if (pBornCopy->brother != nullptr)
	{
		ReleseCopyBornTree(pBornCopy->brother);
		delete pBornCopy->brother;
	}
}