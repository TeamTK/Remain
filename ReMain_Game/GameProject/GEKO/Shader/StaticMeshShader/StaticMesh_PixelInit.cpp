#include "StaticMeshShader.h"

bool StaticMeshShader::InitPixelShader(ID3D11Device *pDevice)
{
	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\MeshHLSL\\Forward\\Mesh.hlsl"), NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &m_pCompiledShader, &m_pErrors, NULL)))
	{
		return false;
	}

	if (FAILED(pDevice->CreatePixelShader(m_pCompiledShader->GetBufferPointer(), m_pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		return false;
	}

	ReleaseBlod();
	return true;
}

bool StaticMeshShader::InitPixelShader_NoTexture(ID3D11Device *pDevice)
{
	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\MeshHLSL\\Forward\\Mesh_NoTexture.hlsl"), NULL, NULL, "PS_NoTexture", "ps_5_0", 0, 0, NULL, &m_pCompiledShader, &m_pErrors, NULL)))
	{
		return false;
	}

	if (FAILED(pDevice->CreatePixelShader(m_pCompiledShader->GetBufferPointer(), m_pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader_NoTexture)))
	{
		return false;
	}

	ReleaseBlod();
	return true;
}

bool StaticMeshShader::InitPixelShader_ShadowMap(ID3D11Device *pDevice)
{
	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\MeshHLSL\\Forward\\Mesh.hlsl"), NULL, NULL, "PS_ShadowMap", "ps_5_0", 0, 0, NULL, &m_pCompiledShader, &m_pErrors, NULL)))
	{
		return false;
	}

	if (FAILED(pDevice->CreatePixelShader(m_pCompiledShader->GetBufferPointer(), m_pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader_ShadowMap)))
	{
		return false;
	}

	ReleaseBlod();
	return true;
}