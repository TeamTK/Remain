#include "SkinMeshData.h"
#include "..\System\Window.h"

SkinMeshData::SkinMeshData()
{
	INIT_NULLPOINTR(m_MeshInfo.pvVertex);
	INIT_NULLPOINTR(m_MeshInfo.m_pMaterial);
	INIT_NULLPOINTR(m_MeshInfo.m_ppIndexBuffer);
	INIT_NULLPOINTR(m_MeshInfo.m_pVertexBuffer);
	INIT_NULLPOINTR(m_MeshInfo.m_pSampleLinear);
}

SkinMeshData::~SkinMeshData()
{
}

const SkinMeshInfo* SkinMeshData::GetSkinMeshInfo() const
{
	return &m_MeshInfo;
}

const BornInfo* SkinMeshData::GetBornInfo() const
{
	return &m_BornInfo;
}

HRESULT SkinMeshData::InitShader()
{
	ID3D11Device *pDevice;
	pDevice = Direct3D11::GetInstance()->GetID3D11Device();

	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.MaxAnisotropy = 4;
	SamDesc.MipLODBias = 0.0f;
	SamDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;;
	SamDesc.MinLOD = -FLT_MAX;
	SamDesc.MaxLOD = FLT_MAX;
	pDevice->CreateSamplerState(&SamDesc, &m_MeshInfo.m_pSampleLinear);

	return S_OK;
}

void SkinMeshData::Relese()
{
	if (m_MeshInfo.m_pMaterial != nullptr)
	{
		for (int i = 0; i < m_MeshInfo.materialNumAll; i++)
		{
			SAFE_RELEASE(m_MeshInfo.m_pMaterial[i].pTexture);
			//SAFE_DELETE_ARRAY(m_MeshInfo.m_pMaterial[i].pPolygonIndex);
			m_MeshInfo.m_ppIndexBuffer[i]->Release();
		}

		SAFE_DELETE_ARRAY(m_MeshInfo.pvVertex);
		SAFE_DELETE_ARRAY(m_MeshInfo.m_pMaterial);
		(*m_MeshInfo.m_ppIndexBuffer)->Release();
		SAFE_DELETE(m_MeshInfo.m_ppIndexBuffer);
		SAFE_RELEASE(m_MeshInfo.m_pVertexBuffer);
		SAFE_RELEASE(m_MeshInfo.m_pSampleLinear);
	}
}

void SkinMeshData::Update(CopyBorn *pCopyBorn)
{
}

void SkinMeshData::BornDebug(eBornDebug eBornDebug)
{
}

void SkinMeshData::AnimationDebug(int animNum)
{
}

void SkinMeshData::CopyBornTree(CopyBorn *pBornCopy, std::vector<CopyBorn*> *pCopyBornArray, Born *pBornOriginal)
{
}