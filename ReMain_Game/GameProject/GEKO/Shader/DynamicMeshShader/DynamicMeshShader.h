#ifndef _DYNAMIC_MESH_SHADER_H_
#define _DYNAMIC_MESH_SHADER_H_

#include "..\..\System\Direct3D11.h"

class DynamicMeshShader
{
public:
	~DynamicMeshShader();
	static DynamicMeshShader* GetInstance();
	bool Init();
	void SetShader(ID3D11DeviceContext *pDeviceContext, bool isTexture, bool isShadow);
	void Release();	

private:
	DynamicMeshShader();
	bool InitVertexShader(ID3D11Device *pDevice);
	bool InitVertexShader_NoTexture(ID3D11Device *pDevice);
	bool InitVertexShader_ShadowMap(ID3D11Device *pDevice);

	bool InitPixelShader(ID3D11Device *pDevice);
	bool InitPixelShader_NoTexture(ID3D11Device *pDevice);
	bool InitPixelShader_ShadowMap(ID3D11Device *pDevice);

private:
	ID3D11InputLayout*	 m_pVertexLayout;				//頂点レイアウト
	ID3D11InputLayout*	 m_pVertexLayout_NoTexture;		//テクスチャーなし頂点レイアウト
	ID3D11VertexShader*  m_pVertexShader;				//頂点シェーダー
	ID3D11VertexShader*  m_pVertexShader_NoTexture;		//テクスチャーなし頂点シェーダー
	ID3D11VertexShader*  m_pVertexShader_ShadowMap;		//シャドウマップ頂点シェーダー
	ID3D11PixelShader*	 m_pPixelShader;				//ピクセルシェーダー
	ID3D11PixelShader*	 m_pPixelShader_NoTexture;		//テクスチャーなしピクセルシェーダー
	ID3D11PixelShader*	 m_pPixelShader_ShadowMap;		//シャドウマップピクセルシェーダー
};

#endif