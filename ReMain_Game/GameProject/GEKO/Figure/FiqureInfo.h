#ifndef _FIQURE_INFO_H_
#define _FIQURE_INFO_H_

#include "..\System\Direct3D11.h"

struct ConstantBufferFiqureBillboard
{
	Matrix mWVP;		//ワールドから射影までの変換行列
	Vector4D color;
};

struct ConstantBufferFiqureBillboardAnimation
{
	Matrix mWVP;		//ワールドから射影までの変換行列
	Vector4D color;
	//ALIGN16 Vector2D uv;
};

struct VertexBillboard
{
	Vector3D pos;
	Vector2D uv;
};

//シェーダーで必要なビルボードの情報
struct FigureBillboardInfo
{
	ID3D11InputLayout*	 pVertexLayout;	 //頂点レイアウト
	ID3D11VertexShader*  pVertexShader;	 //頂点シェーダー
	ID3D11PixelShader*	 pPixelShader;	 //ピクセルシェーダー
	ID3D11Buffer*		 pConstantBuffer;

	FigureBillboardInfo()
	{
		pVertexLayout = nullptr;
		pVertexShader = nullptr;
		pPixelShader = nullptr;
		pConstantBuffer = nullptr;
	}

	~FigureBillboardInfo()
	{
		SAFE_RELEASE(pConstantBuffer);
		SAFE_RELEASE(pVertexShader);
		SAFE_RELEASE(pPixelShader);
		SAFE_RELEASE(pVertexLayout);
	}
};

#endif