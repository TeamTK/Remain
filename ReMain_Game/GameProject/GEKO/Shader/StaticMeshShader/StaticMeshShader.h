#ifndef _STATIC_MESH_SHADER_H_
#define _STATIC_MESH_SHADER_H_

#include "..\..\System\Direct3D11.h"

struct ConstantBuffer2
{

};

//シェーダーに渡す用(変換行列など)
struct ConstantBuffer0
{
	D3DXMATRIX  mW;			//ワールド行列
	D3DXMATRIX  mWVP;		//ワールドから射影までの変換行列
	D3DXMATRIX  mWLP;		//ライトから見た変換行列
	D3DXVECTOR4 LightDir;	//ライト方向
	D3DXVECTOR4 fIntensity; //ライトの明るさ
	D3DXVECTOR4 eyePos;		//カメラ位置
};

//シェーダーに渡す用（マテリアル）
struct ConstantBuffer1
{
	D3DXVECTOR4 ambient;	//アンビエント光
	D3DXVECTOR4 diffuse;	//ディフューズ色
	D3DXVECTOR4 specular;	//鏡面反射
};

class StaticMeshShader
{
public:
	~StaticMeshShader();
	static StaticMeshShader* GetInstance();
	bool Init();
	void BaseConstantBuffer(ID3D11DeviceContext *pDeviceContext, const Matrix &mat, bool isShadow);
	void MaterialConstantBuffer(ID3D11DeviceContext *pDeviceContext, D3DXVECTOR4 &diffuse, D3DXVECTOR4 &specular, D3DXVECTOR4 &ambient);
	void SetVertexShader(ID3D11DeviceContext *pDeviceContext, bool isTexture, bool isShadow);
	void SetPixelShader(ID3D11DeviceContext *pDeviceContext, bool isTexture, bool isShadow);
	void SetInputLayout(ID3D11DeviceContext *pDeviceContext, bool isTexture);
	//void Begin(const char* classInstanceName);
	//void End();
	void Release();	

private:
	StaticMeshShader();
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
	ID3D11Buffer*		 m_pConstantBuffer0;			//コンスタントバッファー（変換行列）
	ID3D11Buffer*		 m_pMaterialConstantBuffer;		//コンスタントバッファー（マテリアル）
	//ID3D11Buffer*		 m_pConstantBuffer2;			//コンスタントバッファー（動的リンク）
	//ID3D11ClassLinkage*  m_pPSClassLinkage;			//動的シェーダーリンク
	//ID3D11ClassInstance* m_pClassInstance;			//ピクセルシェーダーに設定時に使用するクラスインスタンス
};

#endif