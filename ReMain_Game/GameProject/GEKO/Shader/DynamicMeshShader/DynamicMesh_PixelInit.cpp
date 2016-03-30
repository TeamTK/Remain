#include "DynamicMeshShader.h"

bool DynamicMeshShader::InitPixelShader(ID3D11Device *pDevice)
{
	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\MeshHLSL\\Forward\\SkinMesh.hlsl"), NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &m_pCompiledShader, &m_pErrors, NULL)))
	{
		ReleaseBlod();
		return false;
	}

	if (FAILED(pDevice->CreatePixelShader(m_pCompiledShader->GetBufferPointer(), m_pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		ReleaseBlod();
		return false;
	}

	ReleaseBlod();
	return true;
}

bool DynamicMeshShader::InitPixelShader_NoTexture(ID3D11Device *pDevice)
{
	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\MeshHLSL\\Forward\\SkinMesh_NoTexture.hlsl"), NULL, NULL, "PS_NoTexture", "ps_5_0", 0, 0, NULL, &m_pCompiledShader, &m_pErrors, NULL)))
	{
		ReleaseBlod();
		return false;
	}

	if (FAILED(pDevice->CreatePixelShader(m_pCompiledShader->GetBufferPointer(), m_pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader_NoTexture)))
	{
		ReleaseBlod();
		return false;
	}

	ReleaseBlod();
	return true;
}

bool DynamicMeshShader::InitPixelShader_ShadowMap(ID3D11Device *pDevice)
{
	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\MeshHLSL\\Forward\\SkinMesh.hlsl"), NULL, NULL, "PS_ShadowMap", "ps_5_0", 0, 0, NULL, &m_pCompiledShader, &m_pErrors, NULL)))
	{
		ReleaseBlod();
		return false;
	}

	if (FAILED(pDevice->CreatePixelShader(m_pCompiledShader->GetBufferPointer(), m_pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader_ShadowMap)))
	{
		ReleaseBlod();
		return false;
	}

	ReleaseBlod();
	return true;
}