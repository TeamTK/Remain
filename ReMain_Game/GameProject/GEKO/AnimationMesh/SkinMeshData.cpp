#include "SkinMeshData.h"
#include "..\System\Window.h"

SkinMeshData::SkinMeshData()
{
	INIT_NULLPOINTR(m_MeshInfo.pvVertex);
	INIT_NULLPOINTR(m_MeshInfo.m_pMaterial);
	INIT_NULLPOINTR(m_MeshInfo.m_ppIndexBuffer);
	INIT_NULLPOINTR(m_MeshInfo.m_Nothing);
	INIT_NULLPOINTR(m_MeshInfo.m_pConstantBuffer0);
	INIT_NULLPOINTR(m_MeshInfo.m_pConstantBuffer1);
	INIT_NULLPOINTR(m_MeshInfo.m_pVertexShader);
	INIT_NULLPOINTR(m_MeshInfo.m_pPixelShader);
	INIT_NULLPOINTR(m_MeshInfo.m_pVertexBuffer);
	INIT_NULLPOINTR(m_MeshInfo.m_pVertexLayout);
	INIT_NULLPOINTR(m_MeshInfo.m_pSampleLinear);
}

SkinMeshData::~SkinMeshData()
{
}

SkinMeshInfo* SkinMeshData::GetSkinMeshInfo()
{
	return &m_MeshInfo;
}

BornInfo* SkinMeshData::GetBornInfo()
{
	return &m_BornInfo;
}

HRESULT SkinMeshData::InitShader()
{
	ID3D11Device *pDevice;
	pDevice = Direct3D11::Get().GetID3D11Device();

	//hlsl�t�@�C���ǂݍ��݃u���u�쐬
	ID3D10Blob *pCompiledShader = NULL;
	ID3D10Blob *pErrors = NULL;

	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (m_MeshInfo.m_IsTexture)
	{
		if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\SkinMesh.hlsl"), NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(0, TEXT("hlsl�ǂݍ��ݎ��s"), NULL, MB_OK);
			return E_FAIL;
		}
	}
	else
	{
		if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\SkinMesh.hlsl"), NULL, NULL, "VS_NoTeX", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(0, TEXT("hlsl�ǂݍ��ݎ��s"), NULL, MB_OK);
			return E_FAIL;
		}
	}

	if (FAILED(pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_MeshInfo.m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, TEXT("�o�[�e�b�N�X�V�F�[�_�[�쐬���s"), NULL, MB_OK);
		return E_FAIL;
	}

	//���_�C���v�b�g���C�A�E�g���`
	UINT numElements = 0;
	D3D11_INPUT_ELEMENT_DESC layout[5];
	if (m_MeshInfo.m_IsTexture)
	{
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONE_INDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		
		numElements = 5;
		memcpy(layout, tmp, sizeof(D3D11_INPUT_ELEMENT_DESC) * numElements);
	}
	else
	{
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONE_INDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		numElements = 4;
		memcpy(layout, tmp, sizeof(D3D11_INPUT_ELEMENT_DESC) * numElements);
	}

	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(pDevice->CreateInputLayout(
		layout, numElements, pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), &m_MeshInfo.m_pVertexLayout)))
	{
		MessageBox(0, TEXT("���_�C���v�b�g���C�A�E�g�쐬���s"), NULL, MB_OK);
		return FALSE;
	}

	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (m_MeshInfo.m_IsTexture)
	{
		if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\SkinMesh.hlsl"), NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(0, TEXT("hlsl�ǂݍ��ݎ��s"), NULL, MB_OK);
			return E_FAIL;
		}
	}
	else
	{
		if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\SkinMesh.hlsl"), NULL, NULL, "PS_NoTex", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(0, TEXT("hlsl�ǂݍ��ݎ��s"), NULL, MB_OK);
			return E_FAIL;
		}
	}

	if (FAILED(pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_MeshInfo.m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, TEXT("�s�N�Z���V�F�[�_�[�쐬���s"), NULL, MB_OK);
		return E_FAIL;
	}

	//�R���X�^���g�o�b�t�@�[�쐬�@�ϊ��s��n���p
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SkinConstantBuffer0);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_MeshInfo.m_pConstantBuffer0)))
	{
		return E_FAIL;
	}

	//�R���X�^���g�o�b�t�@�[�쐬  �}�e���A���n���p
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SkinConstantBuffer1);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_MeshInfo.m_pConstantBuffer1)))
	{
		return E_FAIL;
	}

	//�R���X�^���g�o�b�t�@�[�{�[���p�쐬  
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(BornConstantBuffer);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_MeshInfo.m_pConstantBufferBone)))
	{
		return E_FAIL;
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	//�e�N�X�`���[�p�T���v���[�쐬
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
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
		SAFE_RELEASE(m_MeshInfo.m_Nothing);
		SAFE_RELEASE(m_MeshInfo.m_pConstantBuffer0);
		SAFE_RELEASE(m_MeshInfo.m_pConstantBuffer1);
		SAFE_RELEASE(m_MeshInfo.m_pConstantBufferBone);
		SAFE_RELEASE(m_MeshInfo.m_pVertexShader);
		SAFE_RELEASE(m_MeshInfo.m_pPixelShader);
		SAFE_RELEASE(m_MeshInfo.m_pVertexBuffer);
		SAFE_RELEASE(m_MeshInfo.m_pVertexLayout);
		SAFE_RELEASE(m_MeshInfo.m_pSampleLinear);
	}
}

int SkinMeshData::GetBornAllNum()
{
	return 0;
}

void SkinMeshData::Update(CopyBorn *pCopyBorn, float *pAinmFrame, int *pAinmNum)
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