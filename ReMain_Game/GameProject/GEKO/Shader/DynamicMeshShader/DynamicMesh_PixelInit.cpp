#include "DynamicMeshShader.h"

bool DynamicMeshShader::InitPixelShader(ID3D11Device *pDevice)
{
	//hlslファイル読み込みブロブ作成
	ID3D10Blob *pCompiledShader = nullptr;
	ID3D10Blob *pErrors = nullptr;

	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\SkinMesh.hlsl"), NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
		return false;
	}

	if (FAILED(pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
		return false;
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);
	return true;
}

bool DynamicMeshShader::InitPixelShader_NoTexture(ID3D11Device *pDevice)
{
	//hlslファイル読み込みブロブ作成
	ID3D10Blob *pCompiledShader = nullptr;
	ID3D10Blob *pErrors = nullptr;

	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\SkinMesh_NoTexture.hlsl"), NULL, NULL, "PS_NoTexture", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
		return false;
	}

	if (FAILED(pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader_NoTexture)))
	{
		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
		return false;
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	return true;
}

bool DynamicMeshShader::InitPixelShader_ShadowMap(ID3D11Device *pDevice)
{
	//hlslファイル読み込みブロブ作成
	ID3D10Blob *pCompiledShader = nullptr;
	ID3D10Blob *pErrors = nullptr;

	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\SkinMesh.hlsl"), NULL, NULL, "PS_ShadowMap", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
		return false;
	}

	if (FAILED(pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader_ShadowMap)))
	{
		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
		return false;
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	return true;
}