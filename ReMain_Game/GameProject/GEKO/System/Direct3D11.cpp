#include "Direct3D11.h"
#include "Window.h"

#define SAMPLE_COUNT 1
#define SAMPLE_QUALITY 0

#include "Debug.h"

Direct3D11::Direct3D11()
{
	INIT_NULLPOINTR(m_pBlendState);
	INIT_NULLPOINTR(m_pBlendState2D);
	INIT_NULLPOINTR(m_pSwapChain);
	INIT_NULLPOINTR(m_pRenderTargetView);
	INIT_NULLPOINTR(m_pDepthStencilView);
	INIT_NULLPOINTR(m_pDepthStencil);
	INIT_NULLPOINTR(m_pDeviceContext);
	INIT_NULLPOINTR(m_pDevice);
}

Direct3D11::~Direct3D11()
{
}

Direct3D11* Direct3D11::GetInstance()
{
	static Direct3D11 direct3d11;
	return &direct3d11;
}

HRESULT Direct3D11::InitD3D11(INT Width, INT Height)
{
	m_ResolutionWidth = Width;
	m_ResolutionHeight = Height;

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
	if(FAILED(InitBackBuffer())) return FALSE;

	//ラスタライズ設定
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	//rdc.CullMode = D3D11_CULL_NONE;
	rdc.CullMode = D3D11_CULL_BACK;
	//rdc.CullMode = D3D11_CULL_FRONT;
	rdc.FillMode = D3D11_FILL_SOLID;
	//rdc.FillMode = D3D11_FILL_WIREFRAME;
	rdc.DepthBias = 0;
	rdc.DepthBiasClamp = 0.0f;
	rdc.SlopeScaledDepthBias = 0.0f;
	rdc.DepthClipEnable = true;
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

	// OMにブレンド・ステート・オブジェクトを設定
	FLOAT BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_pDeviceContext->OMSetBlendState(m_pBlendState, BlendFactor, 0xffffffff);

	////アルファブレンド用ブレンドステート作成2D用
	D3D11_BLEND_DESC bd2D;
	ZeroMemory(&bd2D, sizeof(D3D11_BLEND_DESC));
	bd2D.IndependentBlendEnable = false;
	bd2D.AlphaToCoverageEnable = false;
	bd2D.RenderTarget[0].BlendEnable = true;
	bd2D.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd2D.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd2D.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd2D.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd2D.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd2D.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd2D.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(m_pDevice->CreateBlendState(&bd2D, &m_pBlendState2D)))
	{
		return FALSE;
	}
	
	//深度・ステンシルステートオブジェクト作成
	D3D11_DEPTH_STENCIL_DESC depthStencil;
	depthStencil.DepthEnable = true;
	depthStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencil.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencil.StencilEnable = false;
	depthStencil.StencilReadMask = D3D10_DEFAULT_STENCIL_READ_MASK;
	depthStencil.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	//面が表を向いている時の設定
	depthStencil.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencil.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencil.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//面が裏を向いている時の設定
	depthStencil.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencil.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencil.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencil.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//デプステストONで作成
	if (FAILED(m_pDevice->CreateDepthStencilState(&depthStencil, &m_pDepthStencilState_DepthOn)))
	{
		return FALSE;
	}

	//デプステストOFFで作成
	depthStencil.DepthEnable = false;
	if (FAILED(m_pDevice->CreateDepthStencilState(&depthStencil, &m_pDepthStencilState_DepthOff)))
	{
		return FALSE;
	}

	return S_OK;
}

HRESULT Direct3D11::InitBackBuffer()
{
	//バックバッファーテクスチャーを取得
	ID3D11Texture2D *pBackBufferTexture;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBufferTexture);

	// バック・バッファの情報
	D3D11_TEXTURE2D_DESC descBackBuffer;
	pBackBufferTexture->GetDesc(&descBackBuffer);

	//テクスチャーに対しレンダーターゲットビューを作成
	m_pDevice->CreateRenderTargetView(pBackBufferTexture, NULL, &m_pRenderTargetView);
	SAFE_RELEASE(pBackBufferTexture);

	//デプスステンシルのテクスチャーを作成
	D3D11_TEXTURE2D_DESC descDepth = descBackBuffer;
	//descDepth.Width = Width;
	//descDepth.Height = Height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	//descDepth.SampleDesc.Count = (UINT)SAMPLE_COUNT;
	//descDepth.SampleDesc.Quality = (UINT)SAMPLE_QUALITY;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pDepthStencil);
	m_pDevice->CreateDepthStencilView(m_pDepthStencil, NULL, &m_pDepthStencilView);

	//レンダーターゲットビューとデプスステンシルビューをパイプラインにセット
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	//ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)descBackBuffer.Width;
	vp.Height = (FLOAT)descBackBuffer.Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	m_pDeviceContext->RSSetViewports(1, &vp);

	return S_OK;
}

void Direct3D11::ChangeBlendState3D()
{
	// OMにブレンド・ステート・オブジェクトを設定
	FLOAT BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_pDeviceContext->OMSetBlendState(m_pBlendState, BlendFactor, 0xffffffff);
}

void Direct3D11::ChangeBlendState2D()
{
	// OMにブレンド・ステート・オブジェクトを設定
	FLOAT BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_pDeviceContext->OMSetBlendState(m_pBlendState2D, BlendFactor, 0xffffffff);
}

void Direct3D11::ChangeWindowSize()
{
	// 描画ターゲットを解除する
	m_pDeviceContext->OMSetRenderTargets(0, NULL, NULL);	// 描画ターゲットの解除
	SAFE_RELEASE(m_pRenderTargetView);					    // 描画ターゲット ビューの解放
	SAFE_RELEASE(m_pDepthStencilView);					    // 深度/ステンシル ビューの解放
	SAFE_RELEASE(m_pDepthStencil);						    // 深度/ステンシル テクスチャの解放

	m_pSwapChain->ResizeBuffers(2, m_ResolutionWidth, m_ResolutionHeight, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	InitBackBuffer();
}

void Direct3D11::SetResolution_And_RefreshRate(int width, int height, int refreshRateNum = 60)
{
	m_ResolutionWidth = width;
	m_ResolutionHeight = height;
	/*
	DXGI_MODE_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.RefreshRate.Numerator = refreshRateNum;
	desc.RefreshRate.Denominator = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	m_pSwapChain->ResizeTarget(&desc);
	*/
	ChangeWindowSize();
}

void Direct3D11::SetRasterizer(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode)
{
	//ラスタライズ設定
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = cullMode;
	rdc.FillMode = fillMode;
	rdc.DepthBias = 0;
	rdc.DepthBiasClamp = 0.0f;
	rdc.SlopeScaledDepthBias = 0.0f;
	rdc.DepthClipEnable = true;
	rdc.FrontCounterClockwise = false;
	rdc.MultisampleEnable = true;
	rdc.AntialiasedLineEnable = false;

	ID3D11RasterizerState* pIr = NULL;
	m_pDevice->CreateRasterizerState(&rdc, &pIr);
	m_pDeviceContext->RSSetState(pIr);
	SAFE_RELEASE(pIr);
}

ID3D11Device* Direct3D11::GetID3D11Device() const
{
	return m_pDevice;
}

ID3D11DeviceContext* Direct3D11::GetID3D11DeviceContext() const
{
	return m_pDeviceContext;
}

ID3D11DepthStencilView* Direct3D11::GetDepthStencilView() const
{
	return m_pDepthStencilView;
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

int Direct3D11::GetResolutionWidth() const
{
	return m_ResolutionWidth;
}

int Direct3D11::GetResolutionHeight() const
{
	return m_ResolutionHeight;
}

float Direct3D11::GetClearColorRed() const
{
	return m_Red;
}

float Direct3D11::GetClearColorGreen() const
{
	return m_Green;
}

float Direct3D11::GetClearColorBlue() const
{
	return m_Blue;
}

void Direct3D11::Clear()
{
	//画面クリア
	float ClearColor[4] = { m_Red, m_Green, m_Blue, 1 }; //クリア色RGBAの順
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor); //カラーバッファクリア
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0); //デプスステンシルバッファクリア
}

void Direct3D11::Present()
{
	m_pSwapChain->Present(0, 0); //画面更新（バックバッファをフロントバッファに)
}

void Direct3D11::RenderTarget()
{
	//レンダーターゲットビューとデプスステンシルビューをパイプラインにセット
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
}

void Direct3D11::DepthOn()
{
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState_DepthOn, 0);
}

void Direct3D11::DepthOff()
{
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState_DepthOff, 0);
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

void Direct3D11::SetClearColor(float red, float green, float blue)
{
	m_Red = red;
	m_Green = green;
	m_Blue = blue;
}

void Direct3D11::DestroyD3D11()
{
	if (m_pSwapChain) m_pSwapChain->SetFullscreenState(false, 0);
	if(m_pDeviceContext) m_pDeviceContext->ClearState();

	SAFE_RELEASE(m_pBlendState);
	SAFE_RELEASE(m_pBlendState2D);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencil);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
	OutputDebugString(TEXT("Direct3D11が正常に終了しました\n"));
}