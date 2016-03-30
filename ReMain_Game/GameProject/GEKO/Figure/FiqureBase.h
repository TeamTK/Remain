#ifndef _FIQUREBASE_H_
#define _FIQUREBASE_H_

#include "..\System\Direct3D11.h"
#include "..\Shader\RenderingManager.h"

struct ConstantBufferFiqure
{
	Matrix mWVP;		//ワールドから射影までの変換行列
	Vector4D Color;
};

struct FigureInfo
{
	ID3D11InputLayout*	 pVertexLayout;	 //頂点レイアウト
	ID3D11VertexShader*  pVertexShader;	 //頂点シェーダー
	ID3D11PixelShader*	 pPixelShader;	 //ピクセルシェーダー
	ID3D11Buffer*		 pConstantBuffer;
	ID3D11Buffer*		 pVertexBuffer;	 //頂点バッファー

	FigureInfo()
	{
		pVertexLayout = nullptr;
		pVertexShader = nullptr;
		pPixelShader = nullptr;
		pConstantBuffer = nullptr;
		pVertexBuffer = nullptr;
	}

	~FigureInfo()
	{
		SAFE_RELEASE(pConstantBuffer);
		SAFE_RELEASE(pVertexShader);
		SAFE_RELEASE(pPixelShader);
		SAFE_RELEASE(pVertexBuffer);
		SAFE_RELEASE(pVertexLayout);
	}
};

class FiqureBase
{
public:
	FiqureBase();
	~FiqureBase();

protected:
	virtual HRESULT InitShader();

protected:
	FigureInfo m_FigureInfo;
};

#endif