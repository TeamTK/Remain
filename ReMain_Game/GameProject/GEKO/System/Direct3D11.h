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
private:
	Direct3D11(){};
	ID3D11Device*			m_pDevice;				//ダイレクトｘのドライバー
	ID3D11DeviceContext*	m_pDeviceContext;		//ダイレクトｘのドライバーの設定
	IDXGISwapChain*			m_pSwapChain;			//裏画面描画
	ID3D11Texture2D*		m_pBuckBuffer_DSTex;	//デプスステンシルビュー
	ID3D11RenderTargetView* m_pBackBuffer_TexRTV;	//レンダーターゲットビュー
	ID3D11DepthStencilView* m_pBuckBuffer_DSTexDSV; //デプスステンシルビュー
	ID3D11BlendState*		m_pBlendState;			//ブレンド
	D3D11_VIEWPORT			m_Viewport;

public:
	static Direct3D11& Get();

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

	/// <summary>
	///　画面クリアのRGBを指定
	/// </summary>
	/// <param name="R">
	/// 赤色
	/// </param>
	/// <param name="G">
	/// 緑色
	/// </param>
	/// <param name="B">
	/// 青色
	/// </param>
	/// <returns>
	/// なし
	/// </returns>
	void Clear(float R, float G, float B);

	/// <summary>
	/// 画面更新
	/// </summary>
	/// <returns>
	/// なし
	/// </returns>
	void Present();

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
};

#endif