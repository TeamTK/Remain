#pragma warning(disable : 4005)

#ifndef _DIRECT3D11_H_
#define _DIRECT3D11_H_

#pragma comment(lib,"GEKO\\Lib\\d3d11.lib")
#pragma comment(lib,"GEKO\\Lib\\d3dx10.lib")
#pragma comment(lib,"GEKO\\Lib\\d3dx11.lib")
#pragma comment(lib,"GEKO\\Lib\\d3dCompiler.lib")

#include "..\Include\\d3d11.h"
#include "..\Include\\d3dx10.h"
#include "..\Include\\d3dx11.h"
#include "..\Include\\d3dCompiler.h"

#include "Math.h"

#include "..\System\Debug.h"

#define INIT_NULLPOINTR(x)   x = nullptr;	 
#define SAFE_DELETE(x)       if(x){ delete   x; x = nullptr;}
#define SAFE_DELETE_ARRAY(x) if(x){ delete[] x; x = nullptr;}
#define SAFE_RELEASE(x)		 if(x){ x->Release(); x = nullptr;}
#define ALIGN16				 _declspec(align(16))
#define RGB 0.003921568627451f

class Direct3D11
{
public:
	~Direct3D11();

	static Direct3D11* GetInstance();

	/// <summary>
	/// ダイレクトXの初期化
	/// </summary>
	/// <param name="Width">
	/// ウィンドウサイズ横
	/// </param>
	/// <param name="Height">
	/// ウィンドウサイズ縦
	/// </param>
	/// <returns>
	/// 初期化の成功判断を返す
	/// </returns>
	HRESULT	InitD3D11(INT Width, INT Height);

	HRESULT InitBackBuffer();

	void ChangeBlendState3D();

	void ChangeBlendState2D();

	void ChangeWindowSize();

	void SetResolution_And_RefreshRate(int width, int height, int refreshRateNum);

	void SetRasterizer(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode);

	/// <summary>
	/// ダイレクトXのドライバーを渡す
	/// </summary>
	/// <returns>
	/// ダイレクトXのドライバー
	/// </returns>
	ID3D11Device* GetID3D11Device() const;

	/// <summary>
	/// ダイレクトXのドライバーコンテキストを渡す
	/// </summary>
	/// <returns>
	/// ダイレクトXのドライバーコンテキスト
	/// </returns>
	ID3D11DeviceContext* GetID3D11DeviceContext() const;

	ID3D11BlendState* GetBlendState() const;

	D3D11_VIEWPORT* GetViewportD3D11();

	int GetResolutionWidth() const;

	int GetResolutionHeight() const;

	/// <summary>
	///　画面クリアのRGBを指定
	/// </summary>
	/// <param name="r">
	/// 赤色
	/// </param>
	/// <param name="r">
	/// 緑色
	/// </param>
	/// <param name="r">
	/// 青色
	/// </param>
	/// <returns>
	/// なし
	/// </returns>
	void Clear(float r, float g, float b);

	/// <summary>
	/// 画面更新
	/// </summary>
	/// <returns>
	/// なし
	/// </returns>
	void Present();

	void RederTarget();

	/// <summary>
	/// 画面をフルスクリーンで表示
	/// </summary>
	/// <param name="isFullScreen">
	/// true : フルスクリーン
	/// false : ウィンドウ
	/// </param>
	/// <returns>
	/// なし
	/// </returns>
	void FullScreen(bool isFullScreen);

	/// <summary>
	/// ビューポートを設定
	/// </summary>
	/// <param name="width">
	/// ビューポートの幅
	/// </param>
	/// <param name="height">
	/// ビューポートの高さ
	/// </param>
	/// <param name="x">
	/// ビューポート左上座標X
	/// </param>
	/// <param name="y">
	/// ビューポート左上座標Y
	/// </param>
	/// <returns>
	/// なし
	/// </returns>
	void SetViewport(float width, float height, float x, float y);

	/// <summary>
	/// ダイレクトXのリソースの破棄
	/// </summary>
	/// <returns>
	/// なし
	/// </returns>
	void DestroyD3D11();

private:
	Direct3D11();

private:
	ID3D11Device*			m_pDevice;				//ダイレクトX11のドライバー
	ID3D11DeviceContext*	m_pDeviceContext;		//ダイレクトX11のドライバーの設定
	IDXGISwapChain*			m_pSwapChain;			//スワップチェーン
	ID3D11Texture2D*		m_pDepthStencil;		//深度・ステンシルを作るテクスチャー
	ID3D11RenderTargetView* m_pRenderTargetView;	//レンダーターゲットビュー
	ID3D11DepthStencilView* m_pDepthStencilView;	//深度・ステンシル・ビュー
	ID3D11BlendState*		m_pBlendState;			//ブレンド設定3D
	ID3D11BlendState*		m_pBlendState2D;		//ブレンド設定2D
	D3D11_VIEWPORT			m_Viewport;				//ビューポート
	int m_ResolutionWidth;							//解像度幅
	int m_ResolutionHeight;							//解像度高さ
};

#endif