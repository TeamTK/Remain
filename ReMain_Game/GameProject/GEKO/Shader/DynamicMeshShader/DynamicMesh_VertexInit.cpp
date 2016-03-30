#include "DynamicMeshShader.h"

bool DynamicMeshShader::InitVertexShader(ID3D11Device *pDevice)
{
	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\MeshHLSL\\Forward\\SkinMesh.hlsl"), NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &m_pCompiledShader, &m_pErrors, NULL)))
	{
		return false;
	}

	if (FAILED(pDevice->CreateVertexShader(m_pCompiledShader->GetBufferPointer(), m_pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		return false;
	}

	//頂点インプットレイアウトを定義
	UINT numElements = 0;
	D3D11_INPUT_ELEMENT_DESC layout[5];
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

	//頂点インプットレイアウトを作成
	if (FAILED(pDevice->CreateInputLayout(
		layout, numElements, m_pCompiledShader->GetBufferPointer(),
		m_pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return false;
	}

	ReleaseBlod();
	return true;
}

bool DynamicMeshShader::InitVertexShader_NoTexture(ID3D11Device *pDevice)
{
	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\MeshHLSL\\Forward\\SkinMesh_NoTexture.hlsl"), NULL, NULL, "VS_NoTexture", "vs_5_0", 0, 0, NULL, &m_pCompiledShader, &m_pErrors, NULL)))
	{
		return false;
	}

	if (FAILED(pDevice->CreateVertexShader(m_pCompiledShader->GetBufferPointer(), m_pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader_NoTexture)))
	{
		return false;
	}

	//頂点インプットレイアウトを定義
	UINT numElements = 0;
	D3D11_INPUT_ELEMENT_DESC layout[4];
	D3D11_INPUT_ELEMENT_DESC tmp[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_INDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	numElements = 4;
	memcpy(layout, tmp, sizeof(D3D11_INPUT_ELEMENT_DESC) * numElements);

	//頂点インプットレイアウトを作成
	if (FAILED(pDevice->CreateInputLayout(
		layout, numElements, m_pCompiledShader->GetBufferPointer(),
		m_pCompiledShader->GetBufferSize(), &m_pVertexLayout_NoTexture)))
	{
		return false;
	}

	ReleaseBlod();
	return true;
}

bool DynamicMeshShader::InitVertexShader_ShadowMap(ID3D11Device *pDevice)
{
	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\MeshHLSL\\Forward\\SkinMesh.hlsl"), NULL, NULL, "VS_ShadowMap", "vs_5_0", 0, 0, NULL, &m_pCompiledShader, &m_pErrors, NULL)))
	{
		return false;
	}

	if (FAILED(pDevice->CreateVertexShader(m_pCompiledShader->GetBufferPointer(), m_pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader_ShadowMap)))
	{
		return false;
	}

	ReleaseBlod();
	return true;
}
