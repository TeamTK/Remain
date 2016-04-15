#include "ForwardRendering.h"
#include "..\..\Shader\ShadowMap\ShaderShadowMap.h"
#include "..\..\Mesh\MeshInfo.h"

struct ForwardVertex
{
	Vector3D pos;
	Vector3D normal;
	Vector2D uv;
};

std::string nameVS[] =
{
	"VS",
	"VS_ShadowMap",
};

std::string namePS[] =
{
	"PS",
	"PS_ShadowMap",
	"PS_Fog",
	"PS_ShadowMap_Fog",
};

ForwardRendering::ForwardRendering()
{
}

ForwardRendering::~ForwardRendering()
{
	Release();
}

ForwardRendering *ForwardRendering::GetInstance()
{
	static ForwardRendering ForwardRendering;
	return &ForwardRendering;
}

bool ForwardRendering::Init()
{
	ID3DBlob *pCompiledShader = nullptr;
	ID3DBlob *pErrors = nullptr;

	if (!InitStaticMesh(pCompiledShader, pErrors))
	{
		MessageBox(0, TEXT("StaticMeshShader初期化失敗"), NULL, MB_OK);
		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
		return false;
	}

	if (!InitDynamicMesh(pCompiledShader, pErrors))
	{
		MessageBox(0, TEXT("DynamicMeshShader初期化失敗"), NULL, MB_OK);
		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
		return false;
	}

	if (!InitBoard(pCompiledShader, pErrors))
	{
		MessageBox(0, TEXT("板ポリ初期化失敗"), NULL, MB_OK);
		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
		return false;
	}

	return true;
}

void ForwardRendering::SetStaticMeshShader(ID3D11DeviceContext *pDeviceContext, unsigned int state)
{
	int typeVS = static_cast<int>(ShaderTypeVS::eNormal);
	int typePS = static_cast<int>(ShaderTypePS::eNormal);

	ShaderType(&typeVS, &typePS, state, pDeviceContext);

	pDeviceContext->VSSetShader(m_pVertexShader[typeVS], NULL, 0);
	pDeviceContext->PSSetShader(m_pPixelShader[typePS], NULL, 0);

	pDeviceContext->IASetInputLayout(m_pVertexLayout[typeVS]);

	//プリミティブ・トポロジーをセット
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ForwardRendering::SetDynamicMeshShader(ID3D11DeviceContext *pDeviceContext, unsigned int state)
{
	int typeVS = static_cast<int>(ShaderTypeVS::eNormal);
	int typePS = static_cast<int>(ShaderTypePS::eNormal);

	ShaderType(&typeVS, &typePS, state, pDeviceContext);

	pDeviceContext->VSSetShader(m_pSkinVertexShader[typeVS], NULL, 0);
	pDeviceContext->PSSetShader(m_pSkinPixelShader[typePS], NULL, 0);

	pDeviceContext->IASetInputLayout(m_pSkinVertexLayout[typeVS]);

	//プリミティブ・トポロジーをセット
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ForwardRendering::Rendering()
{
	Direct3D11 *pDirect11 = Direct3D11::GetInstance();
	ID3D11DeviceContext *pDeviceContext = pDirect11->GetID3D11DeviceContext();

	ShaderShadowMap::GetInstance()->SetShadowMap(pDeviceContext);

	pDeviceContext->VSSetShader(m_pVertexShaderBoard, NULL, 0);
	pDeviceContext->PSSetShader(m_pPixelShaderBoard, NULL, 0);

	//pDeviceContext->PSSetShaderResources(0, 1, );
	pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);

	//スクリーンサイズのポリゴン
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	UINT stride = sizeof(ForwardVertex);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	pDeviceContext->IASetInputLayout(m_pVertexLayoutBoard);

	pDeviceContext->Draw(4, 0);
}

void ForwardRendering::Release()
{	
	for (int i = 0; i < SHADER_TYPE_TOTAL_VS; i++)
	{
		//メッシュ用
		SAFE_RELEASE(m_pVertexShader[i]);
		SAFE_RELEASE(m_pVertexLayout[i]);

		//スキンメッシュ用
		SAFE_RELEASE(m_pSkinVertexShader[i]);
		SAFE_RELEASE(m_pSkinVertexLayout[i]);
	}

	for (int i = 0; i < SHADER_TYPE_TOTAL_PS; i++)
	{
		//メッシュ用
		SAFE_RELEASE(m_pPixelShader[i]);

		//スキンメッシュ用
		SAFE_RELEASE(m_pSkinPixelShader[i]);
	}

	SAFE_RELEASE(m_pVertexShaderBoard);
	SAFE_RELEASE(m_pPixelShaderBoard);
	SAFE_RELEASE(m_pVertexLayoutBoard);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pSampleLinear);
}

bool ForwardRendering::InitStaticMesh(ID3DBlob *pCompiledShader, ID3DBlob *pErrors)
{
	ID3D11Device *pDevice = Direct3D11::GetInstance()->GetID3D11Device();

	//頂点インプットレイアウトを定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//頂点シェーダーと頂点レイアウト構築
	for (int i = 0; i < SHADER_TYPE_TOTAL_VS; i++)
	{
		//ブロブからバーテックスシェーダー作成
		if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\MeshHLSL\\Forward\\Mesh.hlsl"), NULL, NULL,
			nameVS[i].data(), "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(0, TEXT((LPCTSTR)pErrors->GetBufferPointer()), NULL, MB_OK);
			return false;
		}

		if (FAILED(pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader[i])))
		{
			return false;
		}

		//頂点インプットレイアウトを作成
		if (FAILED(pDevice->CreateInputLayout(
			layout, 3, pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(), &m_pVertexLayout[i])))
		{
			return false;
		}
	}

	//ピクセルシェーダー構築
	for (int i = 0; i < SHADER_TYPE_TOTAL_PS; i++)
	{
		//ブロブからピクセルシェーダー作成
		if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\MeshHLSL\\Forward\\Mesh.hlsl"), NULL, NULL,
			namePS[i].data(), "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(0, TEXT((LPCTSTR)pErrors->GetBufferPointer()), NULL, MB_OK);
			return false;
		}

		if (FAILED(pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
			NULL, &m_pPixelShader[i])))
		{
			return false;
		}
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);
	return true;
}

bool ForwardRendering::InitDynamicMesh(ID3DBlob *pCompiledShader, ID3DBlob *pErrors)
{
	ID3D11Device *pDevice = Direct3D11::GetInstance()->GetID3D11Device();

	//頂点インプットレイアウトを定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_INDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//頂点シェーダーと頂点レイアウト構築
	for (int i = 0; i < SHADER_TYPE_TOTAL_VS; i++)
	{
		//ブロブからバーテックスシェーダー作成
		if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\MeshHLSL\\Forward\\SkinMesh.hlsl"), NULL, NULL,
			nameVS[i].data(), "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(0, TEXT((LPCTSTR)pErrors->GetBufferPointer()), NULL, MB_OK);
			return false;
		}

		if (FAILED(pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(), NULL, &m_pSkinVertexShader[i])))
		{
			return false;
		}

		//頂点インプットレイアウトを作成
		if (FAILED(pDevice->CreateInputLayout(
			layout, 5, pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(), &m_pSkinVertexLayout[i])))
		{
			return false;
		}
	}

	//ピクセルシェーダー構築
	for (int i = 0; i < SHADER_TYPE_TOTAL_PS; i++)
	{
		//ブロブからピクセルシェーダー作成
		if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\MeshHLSL\\Forward\\SkinMesh.hlsl"), NULL, NULL,
			namePS[i].data(), "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(0, TEXT((LPCTSTR)pErrors->GetBufferPointer()), NULL, MB_OK);
			return false;
		}

		if (FAILED(pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
			NULL, &m_pSkinPixelShader[i])))
		{
			return false;
		}
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);
	return true;
}

bool ForwardRendering::InitBoard(ID3DBlob *pCompiledShader, ID3DBlob *pErrors)
{
	ID3D11Device *pDevice = Direct3D11::GetInstance()->GetID3D11Device();

	D3D11_INPUT_ELEMENT_DESC tmpMesh[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	//頂点シェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO/HLSL/MeshHLSL/Forward/Borad.hlsl"), NULL, NULL, 
		"VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, TEXT((LPCTSTR)pErrors->GetBufferPointer()), NULL, MB_OK);
		return false;
	}

	if (FAILED(pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), NULL, &m_pVertexShaderBoard)))
	{
		return false;
	}

	//頂点インプットレイアウトを作成
	if (FAILED(pDevice->CreateInputLayout(tmpMesh, 3, pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), &m_pVertexLayoutBoard)))
	{
		return false;
	}

	//ピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO/HLSL/MeshHLSL/Forward/Borad.hlsl"),
		NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, TEXT((LPCTSTR)pErrors->GetBufferPointer()), NULL, MB_OK);
		return false;
	}

	if (FAILED(pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
		NULL, &m_pPixelShaderBoard)))
	{
		return false;
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	//頂点Buffer作成
	ForwardVertex VertexData[] =
	{
		Vector3D(-1.0f, -1.0f, 0.0f), Vector3D(0.0f, 0.0f, -1.0f), Vector2D(0.0f, 1.0f),
		Vector3D(-1.0f, 1.0f, 0.0f), Vector3D(0.0f, 0.0f, -1.0f), Vector2D(0.0f, 0.0f),
		Vector3D(1.0f, -1.0f, 0.0f), Vector3D(0.0f, 0.0f,-1.0f), Vector2D(1.0f, 1.0f),
		Vector3D(1.0f, 1.0f, 0.0f), Vector3D(0.0f, 0.0f,-1.0f), Vector2D(1.0f, 0.0f)
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ForwardVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = VertexData;
	pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);

	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.MaxAnisotropy = 1;
	SamDesc.MipLODBias = 0.0f;
	SamDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;;
	SamDesc.MinLOD = -FLT_MAX;
	SamDesc.MaxLOD = FLT_MAX;

	pDevice->CreateSamplerState(&SamDesc, &m_pSampleLinear);

	return true;
}

void ForwardRendering::ShaderType(int *typeVS, int *typePS, unsigned int state, ID3D11DeviceContext *pDeviceContext)
{
	switch (state)
	{
	case eNothing:
	case eBlockingLight:
		break;

	case eShadow:
	case eShadow | eBlockingLight:
		*typeVS = static_cast<int>(ShaderTypeVS::eNormal_Shadow);
		*typePS = static_cast<int>(ShaderTypePS::eNormal_Shadow);

		ShaderShadowMap::GetInstance()->SetShadowMap(pDeviceContext);
		break;

	case eFog:
		*typePS = static_cast<int>(ShaderTypePS::eNormal_Fog);

		break;

	case eFog | eShadow:
	case eFog | eBlockingLight:
	case eFog | eShadow | eBlockingLight:
		*typeVS = static_cast<int>(ShaderTypeVS::eNormal_Shadow);
		*typePS = static_cast<int>(ShaderTypePS::eShadow_Fog);

		ShaderShadowMap::GetInstance()->SetShadowMap(pDeviceContext);
		break;

	default:
		break;
	}
}