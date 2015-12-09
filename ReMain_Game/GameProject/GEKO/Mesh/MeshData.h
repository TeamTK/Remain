#ifndef _MESHDATA_H_
#define _MESHDATA_H_

#include "..\System\Math.h"
#include <string>
#include <assert.h>
#include <vector>

//シェーダーに渡す用(変換行列など)
struct ConstantBuffer0
{
	D3DXMATRIX  mW;			//ワールド行列
	D3DXMATRIX  mWVP;		//ワールドから射影までの変換行列
	D3DXVECTOR4 vLightDir;	//ライト方向
	D3DXVECTOR4 fIntensity; //ライトの明るさ
	D3DXVECTOR4 vEye;		//カメラ位置
};

//シェーダーに渡す用（マテリアル）
struct ConstantBuffer1
{
	D3DXVECTOR4 vAmbient;	//アンビエント光
	D3DXVECTOR4 vDiffuse;	//ディフューズ色
	D3DXVECTOR4 vSpecular;	//鏡面反射
};

//マテリアル構造体
struct MaterialInfo
{
	ID3D11ShaderResourceView* pTexture;
	Vector4D Diffuse;	//ディフューズ
	Vector3D Specular;	//スペキュラー
	Vector3D Ambient;	//アンビエント
	std::string TextureName;//テクスチャーファイル名
	int dwNumFace;		//そのマテリアルであるポリゴン数
	MaterialInfo()
	{
		//ZeroMemory(this, sizeof(MaterialInfo));
		pTexture = nullptr;
		dwNumFace = 0;
	}
	~MaterialInfo()
	{
		//SAFE_RELEASE(pTexture);
	}
};

/*
struct Material
{
	D3DXVECTOR4 Ambient;	//アンビエント
	D3DXVECTOR4 Diffuse;	//ディフューズ
	D3DXVECTOR4 Specular;	//スペキュラー

	Material(D3DXVECTOR4 &Diffuse, D3DXVECTOR4 &Specular, D3DXVECTOR4 &Ambient) :
		Diffuse(Diffuse),
		Specular(Specular),
		Ambient(Ambient)
	{
	}
};

struct TextureInfo
{
	std::string TextureName;//テクスチャーファイル名
	ID3D11ShaderResourceView* pTexture;
	TextureInfo(const std::string &TextureName) : 
		TextureName(TextureName)
	{
	}
	
};
*/

//頂点の構造体
struct VertexInfo
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNormal;
	D3DXVECTOR2 vTex;
};

//メッシュの情報
struct MeshInfo
{
	ID3D11InputLayout*	 m_pVertexLayout;	 //頂点レイアウト
	ID3D11VertexShader*  m_pVertexShader;	 //頂点シェーダー
	ID3D11PixelShader*	 m_pPixelShader;	 //ピクセルシェーダー
	ID3D11Buffer*		 m_pConstantBuffer0; //コンスタントバッファー（変換行列）
	ID3D11Buffer*		 m_pConstantBuffer1; //コンスタントバッファー（マテリアル）
	ID3D11Buffer*		 m_pVertexBuffer;	 //頂点バッファー
	ID3D11Buffer**		 m_ppIndexBuffer;	 //インデックスバッファー
	ID3D11SamplerState*  m_pSampleLinear;	 //テクスチャーのサンプラー
	MaterialInfo*		 m_pMaterial;		 //マテリアル情報
	ID3D11ShaderResourceView* m_Nothing;     //テクスチャーがない場合使用
	bool m_IsTexture;						 //テクスチャー判断
	VertexInfo* pvVertex;					 //頂点情報
	Matrix m_LocalMat;
	int vertexNumAll; //頂点数
	int faceNumAll;   //面の数
	int normalNumAll; //法線の数
	int materialNumAll;  //マテリアルの数
	int materialListNumAll; //マテリアルリストの数;
	int uvNumAll;	  //UVの数
	MeshInfo()
	{
		m_pVertexBuffer = nullptr;
		m_pMaterial = nullptr;
		m_IsTexture = true;
	}
	~MeshInfo()
	{
	}
};

class MeshData
{
public:
	MeshData();
	~MeshData();
	MeshInfo *GetMeshInfo();
	virtual void Relese();

protected:
	HRESULT InitShader();

protected:
	MeshInfo m_MeshInfo;

private:

};

#endif 