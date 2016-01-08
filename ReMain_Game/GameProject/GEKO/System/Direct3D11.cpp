#include "Direct3D11.h"
#include "Window.h"

#define SAMPLE_COUNT 2.9f
#define SAMPLE_QUALITY 0.9f

#include "Debug.h"

Direct3D11& Direct3D11::Get()
{
	static Direct3D11 direct3d11;
	return direct3d11;
}

HRESULT Direct3D11::InitD3D11(INT Width, INT Height)
{
	// デバイスとスワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = Width;
	sd.BufferDesc.Height = Height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = Window::Get()->GetWindowHandle();
	sd.SampleDesc.Count = (UINT)SAMPLE_COUNT;
	sd.SampleDesc.Quality = (UINT)SAMPLE_QUALITY;
	sd.Windowed = TRUE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;

	// デバイスとスワップチェーンの作成
	if (FAILED(D3D11CreateDeviceAndSwapChain(
		NULL, 
		D3D_DRIVER_TYPE_HARDWARE, 
		NULL,
		0, 
		&pFeatureLevels, 
		1, 
		D3D11_SDK_VERSION, 
		&sd, 
		&m_pSwapChain, 
		&m_pDevice,
		pFeatureLevel, 
		&m_pDeviceContext)))
	{
		return FALSE;
	}

	//各種テクスチャーと、それに付帯する各種ビューを作成

	//バックバッファーテクスチャーを取得
	ID3D11Texture2D *pBackBuffer_Tex;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer_Tex);
	//そのテクスチャーに対しレンダーターゲットビュー(RTV)を作成
	m_pDevice->CreateRenderTargetView(pBackBuffer_Tex, NULL, &m_pBackBuffer_TexRTV);
	SAFE_RELEASE(pBackBuffer_Tex);

	//デプスステンシルビュー用のテクスチャーを作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = Width;
	descDepth.Height = Height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = (UINT)SAMPLE_COUNT;
	descDepth.SampleDesc.Quality = (UINT)SAMPLE_QUALITY;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pBuckBuffer_DSTex);
	//そのテクスチャーに対しデプスステンシルビュー(DSV)を作成
	m_pDevice->CreateDepthStencilView(m_pBuckBuffer_DSTex, NULL, &m_pBuckBuffer_DSTexDSV);

	//レンダーターゲットビューとデプスステンシルビューをパイプラインにセット
	m_pDeviceContext->OMSetRenderTargets(1, &m_pBackBuffer_TexRTV, m_pBuckBuffer_DSTexDSV);

	//ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)Width;
	vp.Height = (FLOAT)Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	m_pDeviceContext->RSSetViewports(1, &vp);

	//ラスタライズ設定
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	//rdc.CullMode = D3D11_CULL_NONE;
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	//rdc.FillMode = D3D11_FILL_WIREFRAME;
	rdc.FrontCounterClockwise = false;
	rdc.MultisampleEnable = true;
	rdc.AntialiasedLineEnable = false;

	ID3D11RasterizerState* pIr = NULL;
	m_pDevice->CreateRasterizerState(&rdc, &pIr);
	m_pDeviceContext->RSSetState(pIr);
	SAFE_RELEASE(pIr);

	//アルファブレンド用ブレンドステート作成
	//pngファイル内にアルファ情報がある。アルファにより透過するよう指定している
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	bd.IndependentBlendEnable = false;
	bd.AlphaToCoverageEnable = true;
	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(m_pDevice->CreateBlendState(&bd, &m_pBlendState)))
	{
		return FALSE;
	}

	UINT mask = 0xffffffff;
	Direct3D11::Get().GetID3D11DeviceContext()->OMSetBlendState(m_pBlendState, NULL, mask);

	return S_OK;
}

ID3D11Device* Direct3D11::GetID3D11Device() const
{
	return m_pDevice;
}

ID3D11DeviceContext* Direct3D11::GetID3D11DeviceContext() const
{
	return m_pDeviceContext;
}

ID3D11BlendState* Direct3D11::GetBlendState() const
{
	return m_pBlendState;
}

D3D11_VIEWPORT *Direct3D11::GetViewportD3D11()
{
	UINT num = 1;
	m_pDeviceContext->RSGetViewports(&num, &m_Viewport);
	return &m_Viewport;
}

void Direct3D11::Clear(float R, float G, float B)
{
	//画面クリア
	float ClearColor[4] = { R, G, B, 1 };// クリア色　RGBAの順
	m_pDeviceContext->ClearRenderTargetView(m_pBackBuffer_TexRTV, ClearColor);//カラーバッファクリア
	m_pDeviceContext->ClearDepthStencilView(m_pBuckBuffer_DSTexDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//デプスステンシルバッファクリア
}

void Direct3D11::Present()
{
	m_pSwapChain->Present(0, 0);//画面更新（バックバッファをフロントバッファに)
}

void Direct3D11::FullScreen(bool isFullScreen)
{
	m_pSwapChain->SetFullscreenState(isFullScreen, NULL);
}

void Direct3D11::SetViewport(float width, float height, float x, float y)
{
	//ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = x;
	vp.TopLeftY = y;
	m_pDeviceContext->RSSetViewports(1, &vp);
}

void Direct3D11::DestroyD3D11()
{
	if (m_pSwapChain)
	{
		m_pSwapChain->SetFullscreenState(false, 0);
	}

	SAFE_RELEASE(m_pBlendState);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pBackBuffer_TexRTV);
	SAFE_RELEASE(m_pBuckBuffer_DSTexDSV);
	SAFE_RELEASE(m_pBuckBuffer_DSTex);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
	OutputDebugString(TEXT("Direct3D11が正常に終了しました\n"));
}