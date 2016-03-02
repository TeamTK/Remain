#include "MeshData.h"
#include "..\System\Window.h"

MeshData::MeshData()
{
	INIT_NULLPOINTR(m_MeshInfo.pVertex);
	INIT_NULLPOINTR(m_MeshInfo.pIndex);
	INIT_NULLPOINTR(m_MeshInfo.pMaterial);
	INIT_NULLPOINTR(m_MeshInfo.ppIndexBuffer);
	INIT_NULLPOINTR(m_MeshInfo.pVertexBuffer);
	INIT_NULLPOINTR(m_MeshInfo.pSampleLinear);
}

MeshData::~MeshData()
{
}

MeshInfo* MeshData::GetMeshInfo()
{
	return &m_MeshInfo;
}

HRESULT MeshData::InitShader()
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

	pDevice->CreateSamplerState(&SamDesc, &m_MeshInfo.pSampleLinear);

	return S_OK;
}

void MeshData::Relese()
{
	if (m_MeshInfo.pMaterial != nullptr)
	{
		for (int i = 0; i < m_MeshInfo.materialNumAll; i++)
		{
			SAFE_RELEASE(m_MeshInfo.pMaterial[i].pTexture);
			m_MeshInfo.ppIndexBuffer[i]->Release();
		}

		SAFE_DELETE_ARRAY(m_MeshInfo.pVertex);
		SAFE_DELETE_ARRAY(m_MeshInfo.pIndex);
		SAFE_DELETE_ARRAY(m_MeshInfo.pMaterial);
		SAFE_DELETE_ARRAY(m_MeshInfo.ppIndexBuffer);
		SAFE_RELEASE(m_MeshInfo.pVertexBuffer);
		SAFE_RELEASE(m_MeshInfo.pSampleLinear);
	}
}