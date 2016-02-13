#include "StaticMeshShader.h"

bool StaticMeshShader::InitVertexShader(ID3D11Device *pDevice)
{
	//hlslファイル読み込みブロブ作成
	ID3D10Blob *pCompiledShader = nullptr;
	ID3D10Blob *pErrors = nullptr;

	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\Mesh.hlsl"), NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
		return false;
	}

	if (FAILED(pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
		return false;
	}

	//頂点インプットレイアウトを定義
	UINT numElements = 0;
	D3D11_INPUT_ELEMENT_DESC layout[3];
	D3D11_INPUT_ELEMENT_DESC tmp[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	numElements = 3;
	memcpy(layout, tmp, sizeof(D3D11_INPUT_ELEMENT_DESC)*numElements);

	//頂点インプットレイアウトを作成
	if (FAILED(pDevice->CreateInputLayout(
		layout, numElements, pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
		return false;
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);
	return true;
}

bool StaticMeshShader::InitVertexShader_NoTexture(ID3D11Device *pDevice)
{
	//hlslファイル読み込みブロブ作成
	ID3D10Blob *pCompiledShader = nullptr;
	ID3D10Blob *pErrors = nullptr;

	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\Mesh_NoTexture.hlsl"), NULL, NULL, "VS_NoTexture", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
		return false;
	}

	if (FAILED(pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader_NoTexture)))
	{
		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
		return false;
	}

	//頂点インプットレイアウトを定義
	UINT numElements = 0;
	D3D11_INPUT_ELEMENT_DESC layout[2];
	D3D11_INPUT_ELEMENT_DESC tmp[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	numElements = 2;
	memcpy(layout, tmp, sizeof(D3D11_INPUT_ELEMENT_DESC)*numElements);

	//頂点インプットレイアウトを作成
	if (FAILED(pDevice->CreateInputLayout(
		layout, numElements, pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), &m_pVertexLayout_NoTexture)))
	{
		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
		return false;
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);
	return true;
}

bool StaticMeshShader::InitVertexShader_ShadowMap(ID3D11Device *pDevice)
{
	//hlslファイル読み込みブロブ作成
	ID3D10Blob *pCompiledShader = nullptr;
	ID3D10Blob *pErrors = nullptr;

	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\Mesh.hlsl"), NULL, NULL, "VS_ShadowMap", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
		return false;
	}

	if (FAILED(pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader_ShadowMap)))
	{
		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
		return false;
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);
	return true;
}
