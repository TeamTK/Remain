#include "ShaderShadowMap.h"
#include "../../System/Direct3D11.h"
#include "../../System/Window.h"
#include "../../Mesh/StaticMesh.h"
#include "../../System/Camera.h"
#include <list>

struct ShadowInfo
{
	D3DXMATRIX lightWVP;
	D3DXMATRIX lightWVTP;
};

class ShaderShadowMap::ShadowMapPimpl
{
public:
	ID3D11Texture2D*		  pTexture2D;		//
	ID3D11RenderTargetView*   pRenderTargetView;	//
	ID3D11DepthStencilView*   pStencilView;	//
	ID3D11Texture2D*		  pTexture2D_DepthStencil;		//
	ID3D11ShaderResourceView* pShaderResourceView;
	ID3D11Buffer*             pBuffer;
	ID3D11SamplerState*       pSampleLinear;	 //テクスチャーのサンプラー

	ID3D11VertexShader*  pVertexShader;				 //頂点シェーダー
	//ID3D11VertexShader* pVertexShader_NoTexture;    //テクスチャーなし頂点シェーダー
	ID3D11PixelShader*	 pPixelShader;				 //ピクセルシェーダー
	//ID3D11PixelShader* pPixelShader_NoTexture;	 //テクスチャーなしピクセルシェーダー
	ID3D11InputLayout*	 pVertexLayout;				 //頂点レイアウト
	//ID3D11InputLayout* m_pVertexLayout_NoTexture;	 //テクスチャーなし頂点レイアウト

	std::list<StaticMesh*> ShadowMapList;
	D3DXMATRIX m_View;
	D3DXMATRIX m_Proj;
};

ShaderShadowMap::ShaderShadowMap() :
	m_Width(3840.0f),
	m_Height(2160.0f),
	m_x(0.0f),
	m_y(0.0f),
	m_z(0.0f)
{
	m_pShadowMaPimpl = new ShadowMapPimpl;
}

ShaderShadowMap::~ShaderShadowMap()
{
	m_pShadowMaPimpl->ShadowMapList.clear();
	Release();
	delete m_pShadowMaPimpl;
}

ShaderShadowMap* ShaderShadowMap::GetInstance()
{
	static ShaderShadowMap shaderShadowMap;
	return &shaderShadowMap;
}

D3DXMATRIX *ShaderShadowMap::GetViewMatrix()
{
	return &m_pShadowMaPimpl->m_View;
}

D3DXMATRIX *ShaderShadowMap::GetProjMatrix()
{
	return &m_pShadowMaPimpl->m_Proj;
}

void ShaderShadowMap::SetShadowMap(ID3D11DeviceContext *pDeviceContext)
{
	pDeviceContext->PSSetSamplers(1, 1, &m_pShadowMaPimpl->pSampleLinear);
	pDeviceContext->PSSetShaderResources(1, 1, &m_pShadowMaPimpl->pShaderResourceView);
}

void ShaderShadowMap::SetResolution(float width, float height)
{
	m_Width = width;
	m_Height = height;
}

void ShaderShadowMap::SetPosition(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

bool ShaderShadowMap::Init()
{
	ID3D11Device *pDevice = Direct3D11::GetInstance()->GetID3D11Device();

	//深度マップテクスチャーを作成
	D3D11_TEXTURE2D_DESC tdesc;
	ZeroMemory(&tdesc, sizeof(D3D11_TEXTURE2D_DESC));
	tdesc.Width = (UINT)m_Width;
	tdesc.Height = (UINT)m_Height;
	tdesc.MipLevels = 1;
	tdesc.ArraySize = 1;
	tdesc.MiscFlags = 0;
	tdesc.Format = DXGI_FORMAT_R32_FLOAT;
	tdesc.SampleDesc.Count = 1;
	tdesc.SampleDesc.Quality = 0;
	tdesc.Usage = D3D11_USAGE_DEFAULT;
	tdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tdesc.CPUAccessFlags = 0;

	pDevice->CreateTexture2D(&tdesc, NULL, &m_pShadowMaPimpl->pTexture2D);

	//深度マップテクスチャー用　レンダーターゲットビュー作成
	D3D11_RENDER_TARGET_VIEW_DESC DescRT;
	DescRT.Format = DXGI_FORMAT_R32_FLOAT;
	DescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	DescRT.Texture2D.MipSlice = 0;

	pDevice->CreateRenderTargetView(m_pShadowMaPimpl->pTexture2D, &DescRT, &m_pShadowMaPimpl->pRenderTargetView);

	//深度マップテクスチャをレンダーターゲットにする際のデプスステンシルビュー用のテクスチャーを作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = (UINT)m_Width;
	descDepth.Height = (UINT)m_Height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_R32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	pDevice->CreateTexture2D(&descDepth, NULL, &m_pShadowMaPimpl->pTexture2D_DepthStencil);

	//そのテクスチャーに対しデプスステンシルビュー(DSV)を作成
	pDevice->CreateDepthStencilView(m_pShadowMaPimpl->pTexture2D_DepthStencil, NULL, &m_pShadowMaPimpl->pStencilView);

	//深度マップテクスチャ用　シェーダーリソースビュー(SRV)作成	
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = 1;

	pDevice->CreateShaderResourceView(m_pShadowMaPimpl->pTexture2D, &SRVDesc, &m_pShadowMaPimpl->pShaderResourceView);

	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	SamDesc.MaxAnisotropy = 2;
	SamDesc.MipLODBias = 0.0f;
	SamDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	SamDesc.BorderColor[0] = 1.0f;
	SamDesc.BorderColor[1] = 1.0f;
	SamDesc.BorderColor[2] = 1.0f;
	SamDesc.BorderColor[3] = 1.0f;
	SamDesc.MinLOD = -FLT_MAX;
	SamDesc.MaxLOD = FLT_MAX;

	pDevice->CreateSamplerState(&SamDesc, &m_pShadowMaPimpl->pSampleLinear);


	//頂点シェーダー初期化
	if (!InitVertexShader(pDevice))
	{
		MessageBox(0, TEXT("頂点シェーダー作成失敗"), NULL, MB_OK);
		return false;
	}

	//ピクセルシェーダー初期化
	if (!InitPixelShader(pDevice))
	{
		MessageBox(0, TEXT("ピクセルシェーダー作成失敗"), NULL, MB_OK);
		return false;
	}

	//コンスタントバッファー作成　変換行列渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ShadowInfo);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(pDevice->CreateBuffer(&cb, NULL, &m_pShadowMaPimpl->pBuffer)))
	{
		MessageBox(0, TEXT("コンスタントバッファー作成失敗"), NULL, MB_OK);
		return false;
	}

	return true;
}

void ShaderShadowMap::Release()
{
	SAFE_RELEASE(m_pShadowMaPimpl->pSampleLinear);
	SAFE_RELEASE(m_pShadowMaPimpl->pBuffer);
	SAFE_RELEASE(m_pShadowMaPimpl->pVertexShader);
	SAFE_RELEASE(m_pShadowMaPimpl->pPixelShader);
	SAFE_RELEASE(m_pShadowMaPimpl->pVertexLayout);
	SAFE_RELEASE(m_pShadowMaPimpl->pTexture2D);
	SAFE_RELEASE(m_pShadowMaPimpl->pTexture2D_DepthStencil);
	SAFE_RELEASE(m_pShadowMaPimpl->pShaderResourceView);
	SAFE_RELEASE(m_pShadowMaPimpl->pStencilView);
	SAFE_RELEASE(m_pShadowMaPimpl->pRenderTargetView);
}

void ShaderShadowMap::Update()
{
	ID3D11Device *pDevice = Direct3D11::GetInstance()->GetID3D11Device();
	ID3D11DeviceContext *pDeviceContext = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	//ID3D11ShaderResourceView *nullView = NULL;
	//pDeviceContext->PSSetShaderResources(1, 1, &nullView);

	//このパスで使用するシェーダーの登録
	pDeviceContext->VSSetShader(m_pShadowMaPimpl->pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(m_pShadowMaPimpl->pPixelShader, NULL, 0);
	pDeviceContext->IASetInputLayout(m_pShadowMaPimpl->pVertexLayout);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//ビューポートの設定
	Direct3D11::GetInstance()->SetViewport(m_Width, m_Height, 0.0f, 0.0f);

	//レンダーターゲットを深度テクスチャーに変更
	pDeviceContext->OMSetRenderTargets(1, &m_pShadowMaPimpl->pRenderTargetView, m_pShadowMaPimpl->pStencilView);
	//このレンダーターゲットをクリア
	float ClearColor[4] = { 0,0,0,1 };// クリア色作成　RGBAの順
	pDeviceContext->ClearRenderTargetView(m_pShadowMaPimpl->pRenderTargetView, ClearColor);//画面クリア
	pDeviceContext->ClearDepthStencilView(m_pShadowMaPimpl->pStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);//深度バッファクリア

	// ビュートランスフォーム（視点座標変換）
	D3DXVECTOR3 vEyePt(m_x, m_y, m_z); //カメラ（視点）位置
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//注視位置
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置

	D3DXMatrixLookAtLH(&m_pShadowMaPimpl->m_View, &vEyePt, &vLookatPt, &vUpVec);

	// プロジェクショントランスフォーム（射影変換）
	D3DXMatrixPerspectiveFovLH(&m_pShadowMaPimpl->m_Proj, (FLOAT)D3DX_PI / 6, (FLOAT)m_Width / (FLOAT)m_Height, 1.0f, 4000.0f);

	Direct3D11::GetInstance()->SetRasterizer(D3D11_CULL_NONE, D3D11_FILL_SOLID);

	//フレームバッファに書き込む
	for(auto& i : m_pShadowMaPimpl->ShadowMapList)
	{
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(pDeviceContext->Map(m_pShadowMaPimpl->pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			ShadowInfo sg;

			sg.lightWVP = i->m_pMeshData->GetMeshInfo()->m_LocalMat * i->m_Matrix * m_pShadowMaPimpl->m_View * m_pShadowMaPimpl->m_Proj;
			D3DXMatrixTranspose(&sg.lightWVP, &sg.lightWVP);

			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(ShadowInfo));
			pDeviceContext->Unmap(m_pShadowMaPimpl->pBuffer, 0);
		}
		pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShadowMaPimpl->pBuffer);
		pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShadowMaPimpl->pBuffer);

		//バーテックスバッファーをセット
		UINT Stride = sizeof(VertexInfo);
		UINT offset = 0;
		pDeviceContext->IASetVertexBuffers(0, 1, &i->m_pMeshData->GetMeshInfo()->m_pVertexBuffer, &Stride, &offset);

		int num = i->m_pMeshData->GetMeshInfo()->materialNumAll;
		for (int j = 0; j < num; j++)
		{
			//使用されていないマテリアル対策
			if (i->m_pMeshData->GetMeshInfo()->m_pMaterial[j].dwNumFace == 0) continue;
			//インデックスバッファーをセット
			pDeviceContext->IASetIndexBuffer(i->m_pMeshData->GetMeshInfo()->m_ppIndexBuffer[j], DXGI_FORMAT_R32_UINT, 0);

			//テクスチャーをシェーダーに渡す
			if (i->m_pMeshData->GetMeshInfo()->m_pMaterial[j].pTexture)
			{
				pDeviceContext->PSSetSamplers(0, 1, &i->m_pMeshData->GetMeshInfo()->m_pSampleLinear);
				pDeviceContext->PSSetShaderResources(0, 1, &i->m_pMeshData->GetMeshInfo()->m_pMaterial[j].pTexture);
			}

			//プリミティブをレンダリング
			pDeviceContext->DrawIndexed(i->m_pMeshData->GetMeshInfo()->m_pMaterial[j].dwNumFace * 3, 0, 0);
		}
	}

	Direct3D11::GetInstance()->SetRasterizer(D3D11_CULL_BACK, D3D11_FILL_SOLID);

	//通常に戻す
	pDeviceContext->VSSetShader(NULL, NULL, 0);
	pDeviceContext->PSSetShader(NULL, NULL, 0);
	float width = (float)Direct3D11::GetInstance()->GetResolutionWidth();
	float height = (float)Direct3D11::GetInstance()->GetResolutionHeight();
	Direct3D11::GetInstance()->SetViewport(width, height, 0.0f, 0.0f);
	Direct3D11::GetInstance()->RederTarget();
}

void ShaderShadowMap::Add(StaticMesh *pStaticMesh)
{
	m_pShadowMaPimpl->ShadowMapList.push_back(pStaticMesh);
}

void ShaderShadowMap::Clear(StaticMesh *pStaticMesh)
{
	auto it = m_pShadowMaPimpl->ShadowMapList.begin();
	auto itEnd = m_pShadowMaPimpl->ShadowMapList.end();

	for (; it != itEnd; it++)
	{
		if (*it == pStaticMesh)
		{
			it = m_pShadowMaPimpl->ShadowMapList.erase(it);
			break;
		}
	}
}

bool ShaderShadowMap::InitVertexShader(ID3D11Device *pDevice)
{
	//hlslファイル読み込みブロブ作成
	ID3D10Blob *pCompiledShader = nullptr;
	ID3D10Blob *pErrors = nullptr;

	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\ShadowMap.hlsl"), NULL, NULL, "VS", "vs_5_0",
									 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
		return false;
	}

	if (FAILED(pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
										   NULL, &m_pShadowMaPimpl->pVertexShader)))
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
		pCompiledShader->GetBufferSize(), &m_pShadowMaPimpl->pVertexLayout)))
	{
		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
		return false;
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);

	return true;
}

bool ShaderShadowMap::InitPixelShader(ID3D11Device *pDevice)
{
	//hlslファイル読み込みブロブ作成
	ID3D10Blob *pCompiledShader = nullptr;
	ID3D10Blob *pErrors = nullptr;

	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(TEXT("GEKO\\HLSL\\ShadowMap.hlsl"), NULL, NULL, "PS", "ps_5_0",
									 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
		return false;
	}

	if (FAILED(pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
										  NULL, &m_pShadowMaPimpl->pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		SAFE_RELEASE(pErrors);
		return false;
	}

	SAFE_RELEASE(pCompiledShader);
	SAFE_RELEASE(pErrors);
	return true;
}