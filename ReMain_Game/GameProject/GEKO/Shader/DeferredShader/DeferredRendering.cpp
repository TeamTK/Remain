#include "DeferredRendering.h"
#include "../ShaderInfo.h"

#define MULTI_RENDER_TOTAL 3

struct DeferredVertex
{
	Vector3D pos;
	Vector3D normal;
	Vector2D uv;
};

struct DeferredRendering::DeferredPimpl
{
	//パス1用通常メッシュ
	ID3D11VertexShader *pVertexShaderPass1[SHADER_TYPE_TOTAL];
	ID3D11PixelShader *pPixelShaderPass1[SHADER_TYPE_TOTAL];
	ID3D11InputLayout *pVertexLayoutPass1[SHADER_TYPE_TOTAL];

	//パス1用スキンメッシュ
	ID3D11VertexShader *pSkinVertexShaderPass1[SHADER_TYPE_TOTAL];
	ID3D11PixelShader *pSkinPixelShaderPass1[SHADER_TYPE_TOTAL];
	ID3D11InputLayout *pSkinVertexLayoutPass1[SHADER_TYPE_TOTAL];

	//マルチレンダリング
	ID3D11Texture2D *pTexture2D[MULTI_RENDER_TOTAL];
	ID3D11RenderTargetView *pRenderTargetView[MULTI_RENDER_TOTAL];
	ID3D11ShaderResourceView *pShaderResourceView[MULTI_RENDER_TOTAL];

	//パス２用
	ID3D11VertexShader *pVertexShaderPass2;
	ID3D11PixelShader *pPixelShaderPass2;
	ID3D11InputLayout *pVertexLayoutPass2;
	ID3D11Buffer *pVertexBuffer;
	ID3D11SamplerState* pSampleLinear;
};

DeferredRendering::DeferredRendering()
{
	m_pDeferredPimpl = new DeferredPimpl;
	for (int i = 0; i < MULTI_RENDER_TOTAL; i++)
	{
		INIT_NULLPOINTR(m_pDeferredPimpl->pTexture2D[i]);
		INIT_NULLPOINTR(m_pDeferredPimpl->pRenderTargetView[i]);
		INIT_NULLPOINTR(m_pDeferredPimpl->pShaderResourceView[i]);
	}

	if (!InitSheder())
	{
		MessageBox(0, TEXT("デファードレンダリングシェーダー作成失敗"), NULL, MB_OK);
	}	
}

DeferredRendering::~DeferredRendering()
{
	Release();
	delete m_pDeferredPimpl;
}

DeferredRendering *DeferredRendering::GetInstance()
{
	static DeferredRendering deferredRendering;
	return &deferredRendering;
}

bool DeferredRendering::Init()
{
	for (int i = 0; i < MULTI_RENDER_TOTAL; i++)
	{
		SAFE_RELEASE(m_pDeferredPimpl->pTexture2D[i]);
		SAFE_RELEASE(m_pDeferredPimpl->pRenderTargetView[i]);
		SAFE_RELEASE(m_pDeferredPimpl->pShaderResourceView[i]);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	ZeroMemory(&RTVDesc, sizeof(RTVDesc));

	auto *pDevice = Direct3D11::GetInstance()->GetID3D11Device();

	//アルべド
	desc.Width = Direct3D11::GetInstance()->GetResolutionWidth();
	desc.Height = Direct3D11::GetInstance()->GetResolutionHeight();
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	RTVDesc.Format = desc.Format;
	RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RTVDesc.Texture2D.MipSlice = 0;

	SRVDesc.Format = desc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	pDevice->CreateTexture2D(&desc, NULL, &m_pDeferredPimpl->pTexture2D[eAlbedo]);
	pDevice->CreateRenderTargetView(m_pDeferredPimpl->pTexture2D[eAlbedo], &RTVDesc, &m_pDeferredPimpl->pRenderTargetView[eAlbedo]);
	pDevice->CreateShaderResourceView(m_pDeferredPimpl->pTexture2D[eAlbedo], &SRVDesc, &m_pDeferredPimpl->pShaderResourceView[eAlbedo]);

	//法線
	desc.Format = DXGI_FORMAT_R10G10B10A2_UNORM;
	RTVDesc.Format = desc.Format;
	SRVDesc.Format = desc.Format;

	pDevice->CreateTexture2D(&desc, NULL, &m_pDeferredPimpl->pTexture2D[eNormal]);
	pDevice->CreateRenderTargetView(m_pDeferredPimpl->pTexture2D[eNormal], &RTVDesc, &m_pDeferredPimpl->pRenderTargetView[eNormal]);
	pDevice->CreateShaderResourceView(m_pDeferredPimpl->pTexture2D[eNormal], &SRVDesc, &m_pDeferredPimpl->pShaderResourceView[eNormal]);

	//座標
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	RTVDesc.Format = desc.Format;
	SRVDesc.Format = desc.Format;

	pDevice->CreateTexture2D(&desc, NULL, &m_pDeferredPimpl->pTexture2D[ePosition]);
	pDevice->CreateRenderTargetView(m_pDeferredPimpl->pTexture2D[ePosition], &RTVDesc, &m_pDeferredPimpl->pRenderTargetView[ePosition]);
	pDevice->CreateShaderResourceView(m_pDeferredPimpl->pTexture2D[ePosition], &SRVDesc, &m_pDeferredPimpl->pShaderResourceView[ePosition]);

	return true;
}

void DeferredRendering::ChangeRenderTarget()
{
	Direct3D11 *pDirect11 = Direct3D11::GetInstance();

	auto *pDeviceContext = pDirect11->GetID3D11DeviceContext();
	auto *pDepthStencilView = pDirect11->GetDepthStencilView();
	float red = pDirect11->GetClearColorRed();
	float green = pDirect11->GetClearColorGreen();
	float blue = pDirect11->GetClearColorBlue();

	pDeviceContext->OMSetRenderTargets(MULTI_RENDER_TOTAL, m_pDeferredPimpl->pRenderTargetView, pDepthStencilView);

	float ClearColor[4] = { red, green, blue, 1 };
	pDeviceContext->ClearRenderTargetView(m_pDeferredPimpl->pRenderTargetView[eAlbedo], ClearColor);
	pDeviceContext->ClearRenderTargetView(m_pDeferredPimpl->pRenderTargetView[eNormal], ClearColor);
	pDeviceContext->ClearRenderTargetView(m_pDeferredPimpl->pRenderTargetView[ePosition], ClearColor);
	pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DeferredRendering::SetShaderMeshPass1(ShaderType type)
{
	Direct3D11 *pDirect11 = Direct3D11::GetInstance();
	auto *pDeviceContext = pDirect11->GetID3D11DeviceContext();

	int typeNum = static_cast<int>(type);

	//パス1で使うシェーダー
	pDeviceContext->VSSetShader(m_pDeferredPimpl->pVertexShaderPass1[typeNum], NULL, 0);
	pDeviceContext->PSSetShader(m_pDeferredPimpl->pPixelShaderPass1[typeNum], NULL, 0);
}

void DeferredRendering::SetShaderSkinMeshPass1(ShaderType type)
{
	Direct3D11 *pDirect11 = Direct3D11::GetInstance();
	auto *pDeviceContext = pDirect11->GetID3D11DeviceContext();

	int typeNum = static_cast<int>(type);

	//パス1で使うシェーダー
	pDeviceContext->VSSetShader(m_pDeferredPimpl->pSkinVertexShaderPass1[typeNum], NULL, 0);
	pDeviceContext->PSSetShader(m_pDeferredPimpl->pSkinPixelShaderPass1[typeNum], NULL, 0);
}

void DeferredRendering::RenderingPass2()
{
	Direct3D11 *pDirect11 = Direct3D11::GetInstance();
	ID3D11DeviceContext *pDeviceContext = pDirect11->GetID3D11DeviceContext();

	pDirect11->RenderTarget();
	pDirect11->Clear();

	//パス2で使うシェーダー
	pDeviceContext->VSSetShader(m_pDeferredPimpl->pVertexShaderPass2, NULL, 0);
	pDeviceContext->PSSetShader(m_pDeferredPimpl->pPixelShaderPass2, NULL, 0);

	//パス1で作成したテクスチャーをセット
	pDeviceContext->PSSetShaderResources(3, 1, &m_pDeferredPimpl->pShaderResourceView[eAlbedo]);
	pDeviceContext->PSSetShaderResources(4, 1, &m_pDeferredPimpl->pShaderResourceView[eNormal]);
	pDeviceContext->PSSetShaderResources(5, 1, &m_pDeferredPimpl->pShaderResourceView[ePosition]);

	pDeviceContext->PSSetSamplers(3, 1, &m_pDeferredPimpl->pSampleLinear);

	//スクリーンサイズのポリゴン
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	UINT stride = sizeof(DeferredVertex);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pDeferredPimpl->pVertexBuffer, &stride, &offset);

	pDeviceContext->IASetInputLayout(m_pDeferredPimpl->pVertexLayoutPass2);

	pDeviceContext->Draw(4, 0);

	//テクスチャリソース初期化
	ID3D11ShaderResourceView* pShaderResource = nullptr;
	pDeviceContext->PSSetShaderResources(3, 1, &pShaderResource);
	pDeviceContext->PSSetShaderResources(4, 1, &pShaderResource);
	pDeviceContext->PSSetShaderResources(5, 1, &pShaderResource);
}

void DeferredRendering::Release()
{
	//パス１
	for (int i = 0; i < SHADER_TYPE_TOTAL; i++)
	{
		//通常メッシュ
		SAFE_RELEASE(m_pDeferredPimpl->pVertexShaderPass1[i]);
		SAFE_RELEASE(m_pDeferredPimpl->pPixelShaderPass1[i]);
		SAFE_RELEASE(m_pDeferredPimpl->pVertexLayoutPass1[i]);

		//スキンメッシュ
		SAFE_RELEASE(m_pDeferredPimpl->pSkinVertexShaderPass1[i]);
		SAFE_RELEASE(m_pDeferredPimpl->pSkinPixelShaderPass1[i]);
		SAFE_RELEASE(m_pDeferredPimpl->pSkinVertexLayoutPass1[i]);
	}

	//マルチパスレンダリング
	for (int i = 0; i < MULTI_RENDER_TOTAL; i++)
	{
		SAFE_RELEASE(m_pDeferredPimpl->pTexture2D[i]);
		SAFE_RELEASE(m_pDeferredPimpl->pRenderTargetView[i]);
		SAFE_RELEASE(m_pDeferredPimpl->pShaderResourceView[i]);
	}

	//パス２
	SAFE_RELEASE(m_pDeferredPimpl->pVertexShaderPass2);
	SAFE_RELEASE(m_pDeferredPimpl->pPixelShaderPass2);
	SAFE_RELEASE(m_pDeferredPimpl->pVertexLayoutPass2);
	SAFE_RELEASE(m_pDeferredPimpl->pVertexBuffer);
	SAFE_RELEASE(m_pDeferredPimpl->pSampleLinear);	
}

bool DeferredRendering::InitSheder()
{
	auto *pDevice = Direct3D11::GetInstance()->GetID3D11Device();

	std::string nameVS[] =
	{
		"VS",
		"VS_ShadowMap"
	};

	std::string namePS[] =
	{
		"PS",
		"PS_ShadowMap"
	};

	ID3DBlob *pCompiledShader = nullptr;
	ID3DBlob *pErrors = nullptr;
	HRESULT hr;

	ID3DBlob *pCompiledShaderMesh[SHADER_TYPE_TOTAL];
	ID3DBlob *pCompiledShaderSkinMesh[SHADER_TYPE_TOTAL];

	//パス１で使うシェーダーを読み込み&構築
	for (int i = 0; i < SHADER_TYPE_TOTAL; i++)
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//ピクセルシェーダー読み込み＆構築
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//ピクセルシェーダー作成パス１(メッシュ)
		hr = D3DX11CompileFromFile(TEXT("GEKO/HLSL/MeshHLSL/Deferred/Deferred_Pass1_Mesh.hlsl"),
			NULL, NULL, namePS[i].data(), "ps_5_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL);
		if (FAILED(hr)) 
		{
			MessageBox(0, TEXT((LPCTSTR)pErrors->GetBufferPointer()), NULL, MB_OK);
			return false;
		}

		hr = pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
			NULL, &m_pDeferredPimpl->pPixelShaderPass1[i]);
		if (FAILED(hr)) return false;

		//ピクセルシェーダー作成パス１(スキン)
		hr = D3DX11CompileFromFile(TEXT("GEKO/HLSL/MeshHLSL/Deferred/Deferred_Pass1_SkinMesh.hlsl"),
			NULL, NULL, namePS[i].data(), "ps_5_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL);
		if (FAILED(hr)) 
		{
			MessageBox(0, TEXT((LPCTSTR)pErrors->GetBufferPointer()), NULL, MB_OK);
			return false;
		}

		hr = pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
			NULL, &m_pDeferredPimpl->pSkinPixelShaderPass1[i]);
		if (FAILED(hr)) return false;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//頂点シェーダー読み込み＆構築
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//頂点シェーダー作成パス1
		hr = D3DX11CompileFromFile(TEXT("GEKO/HLSL/MeshHLSL/Deferred/Deferred_Pass1_Mesh.hlsl"),
			NULL, NULL, nameVS[i].data(), "vs_5_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShaderMesh[i], &pErrors, NULL);
		if (FAILED(hr)) 
		{
			MessageBox(0, TEXT((LPCTSTR)pErrors->GetBufferPointer()), NULL, MB_OK);
			return false;
		}

		hr = pDevice->CreateVertexShader(pCompiledShaderMesh[i]->GetBufferPointer(), pCompiledShaderMesh[i]->GetBufferSize(),
			NULL, &m_pDeferredPimpl->pVertexShaderPass1[i]);
		if (FAILED(hr)) return false;

		//スキン頂点シェーダー作成パス1
		hr = D3DX11CompileFromFile(TEXT("GEKO/HLSL/MeshHLSL/Deferred/Deferred_Pass1_SkinMesh.hlsl"),
			NULL, NULL, nameVS[i].data(), "vs_5_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShaderSkinMesh[i], &pErrors, NULL);
		if (FAILED(hr))
		{
			MessageBox(0, TEXT((LPCTSTR)pErrors->GetBufferPointer()), NULL, MB_OK);
			return false;
		}

		hr = pDevice->CreateVertexShader(pCompiledShaderSkinMesh[i]->GetBufferPointer(), pCompiledShaderSkinMesh[i]->GetBufferSize(),
			NULL, &m_pDeferredPimpl->pSkinVertexShaderPass1[i]);
		if (FAILED(hr)) return false;
	}

	D3D11_INPUT_ELEMENT_DESC tmpMesh[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3D11_INPUT_ELEMENT_DESC tmpSkinMesh[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_INDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//頂点レイアウト構築
	for (int i = 0; i < SHADER_TYPE_TOTAL; i++)
	{
		//メッシュ頂点インプットレイアウトを作成
		hr = pDevice->CreateInputLayout(tmpMesh, 3, pCompiledShaderMesh[i]->GetBufferPointer(),
			 pCompiledShaderMesh[i]->GetBufferSize(), &m_pDeferredPimpl->pVertexLayoutPass1[i]);
		if (FAILED(hr)) return false;

		//スキンメッシュ頂点インプットレイアウトを作成
		hr = pDevice->CreateInputLayout(tmpSkinMesh, 5, pCompiledShaderSkinMesh[i]->GetBufferPointer(),
			 pCompiledShaderSkinMesh[i]->GetBufferSize(), &m_pDeferredPimpl->pSkinVertexLayoutPass1[i]);
		if (FAILED(hr)) return false;
	}

	for (int i = 0; i < SHADER_TYPE_TOTAL; i++)
	{
		SAFE_RELEASE(pCompiledShaderMesh[i]);
		SAFE_RELEASE(pCompiledShaderSkinMesh[i]);
	}

	//頂点シェーダー作成パス2
	hr = D3DX11CompileFromFile(TEXT("GEKO/HLSL/MeshHLSL/Deferred/Deferred_Pass2.hlsl"),
		 NULL, NULL, "VS", "vs_5_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL);
	if (FAILED(hr)) 
	{
		MessageBox(0, TEXT((LPCTSTR)pErrors->GetBufferPointer()), NULL, MB_OK);
		return false;
	}

	hr = pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
		 NULL, &m_pDeferredPimpl->pVertexShaderPass2);
	if (FAILED(hr)) return false;

	//頂点インプットレイアウトを作成
	hr = pDevice->CreateInputLayout(tmpMesh, 3, pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), &m_pDeferredPimpl->pVertexLayoutPass2);
	if (FAILED(hr)) return false;

	//ピクセルシェーダー作成パス２
	hr = D3DX11CompileFromFile(TEXT("GEKO/HLSL/MeshHLSL/Deferred/Deferred_Pass2.hlsl"),
		 NULL, NULL, "PS", "ps_5_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL);
	if (FAILED(hr))
	{
		MessageBox(0, TEXT((LPCTSTR)pErrors->GetBufferPointer()), NULL, MB_OK);
		return false;
	}

	hr = pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
		 NULL, &m_pDeferredPimpl->pPixelShaderPass2);
	if (FAILED(hr)) return false;

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	//頂点Buffer作成
	DeferredVertex VertexData[] =
	{
		Vector3D(-1.0f, -1.0f, 0.0f), Vector3D(0.0f, 0.0f, -1.0f), Vector2D(0.0f, 1.0f),
		Vector3D(-1.0f, 1.0f, 0.0f), Vector3D(0.0f, 0.0f, -1.0f), Vector2D(0.0f, 0.0f),
		Vector3D(1.0f, -1.0f, 0.0f), Vector3D(0.0f, 0.0f,-1.0f), Vector2D(1.0f, 1.0f),
		Vector3D(1.0f, 1.0f, 0.0f), Vector3D(0.0f, 0.0f,-1.0f), Vector2D(1.0f, 0.0f)
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(DeferredVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = VertexData;
	hr = pDevice->CreateBuffer(&bd, &InitData, &m_pDeferredPimpl->pVertexBuffer);

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

	pDevice->CreateSamplerState(&SamDesc, &m_pDeferredPimpl->pSampleLinear);

	if (FAILED(hr)) return false;
	return true;
}