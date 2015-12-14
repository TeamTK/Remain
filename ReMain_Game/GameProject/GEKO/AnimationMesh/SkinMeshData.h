#ifndef _SKINMESH_DATA_H_
#define _SKINMESH_DATA_H_

#include "..\System\Math.h"
#include <string>
#include <vector>
#include <fstream>
#include <map>

#define MAX_BONES 255

//シェーダーに渡す用(変換行列など)
struct SkinConstantBuffer0
{
	D3DXMATRIX  mW;			//ワールド行列
	D3DXMATRIX  mWVP;		//ワールドから射影までの変換行列
	D3DXVECTOR4 vLightDir;	//ライト方向
	D3DXVECTOR4 fIntensity; //ライトの明るさ
	D3DXVECTOR4 vEye;		//カメラ位置
};

//シェーダーに渡す用（マテリアル）
struct SkinConstantBuffer1
{
	D3DXVECTOR4 vAmbient;	//アンビエント光
	D3DXVECTOR4 vDiffuse;	//ディフューズ色
	D3DXVECTOR4 vSpecular;	//鏡面反射
};

//シェーダーに渡す用（ボーン）
struct BornConstantBuffer
{
	D3DXMATRIX bornMat[MAX_BONES];
	BornConstantBuffer()
	{
		for (int i = 0; i < MAX_BONES; i++)
		{
			D3DXMatrixIdentity(&bornMat[i]);
		}
	}
};

//マテリアル構造体
struct SkinMaterialInfo
{
	ID3D11ShaderResourceView* pTexture;
	Vector4D Diffuse;	//ディフューズ
	Vector3D Specular;	//スペキュラー
	Vector3D Ambient;	//アンビエント
	std::string TextureName;//テクスチャーファイル名
	int dwNumFace;		//そのマテリアルであるポリゴン数
	SkinMaterialInfo()
	{
		pTexture = nullptr;
		dwNumFace = 0;
	}
	~SkinMaterialInfo()
	{
	}
};

//頂点の構造体
struct SkinVertexInfo
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNormal;
	D3DXVECTOR2 vTex;
	UINT BoneIndex[4];	 //ボーン番号
	float BoneWeight[4];//ボーン重み
	SkinVertexInfo()
	{
		BoneIndex[0] = 0;
		BoneIndex[1] = 0;
		BoneIndex[2] = 0;
		BoneIndex[3] = 0;

		BoneWeight[0] = 0.0f;
		BoneWeight[1] = 0.0f;
		BoneWeight[2] = 0.0f;
		BoneWeight[3] = 0.0f;
	}
};

enum eBornDebug
{
	eInitMat,
	eOffsetMat,
	eWorld,
	eBornMat
};

//スキンメッシュの情報
struct SkinMeshInfo
{
	ID3D11InputLayout*	 m_pVertexLayout;	 //頂点レイアウト
	ID3D11VertexShader*  m_pVertexShader;	 //頂点シェーダー
	ID3D11PixelShader*	 m_pPixelShader;	 //ピクセルシェーダー
	ID3D11Buffer*		 m_pConstantBuffer0; //コンスタントバッファー（変換行列）
	ID3D11Buffer*		 m_pConstantBuffer1; //コンスタントバッファー（マテリアル）
	ID3D11Buffer*		 m_pVertexBuffer;	 //頂点バッファー
	ID3D11Buffer**		 m_ppIndexBuffer;	 //インデックスバッファー
	ID3D11Buffer*		 m_pConstantBufferBone;
	ID3D11SamplerState*  m_pSampleLinear;	 //テクスチャーのサンプラー
	SkinMaterialInfo*	 m_pMaterial;		 //マテリアル情報
	ID3D11ShaderResourceView* m_Nothing;     //テクスチャーがない場合使用
	DWORD				 m_dwNumMaterial;	 //マテリアルの数
	bool m_IsTexture;						 //テクスチャー判断
	SkinVertexInfo* pvVertex;				 //頂点情報
	BornConstantBuffer BornBuffer;
	int vertexNumAll; //頂点数
	int faceNumAll;   //面の数
	int normalNumAll; //法線の数
	int materialNumAll;  //マテリアルの数
	int materialListNumAll; //マテリアルリストの数;
	int uvNumAll;	  //UVの数
	SkinMeshInfo()
	{
		m_pVertexBuffer = nullptr;
		m_pMaterial = nullptr;
	}
	~SkinMeshInfo()
	{
	}
};

//ボーン
struct Born
{
	int indexId;
	Born *brother;
	Born *child;
	std::string BornName;
	D3DXMATRIX initMat;
	D3DXMATRIX offsetMat;
	D3DXMATRIX worldMat;
	D3DXMATRIX bornMat;
	D3DXMATRIX ParentAndChildMat;
	Born()
	{
		brother = nullptr;
		child = nullptr;
	}
};

//ボーン情報
struct BornInfo
{
	Born sBorn;		  //ボーン階層構造
	std::vector<Born *> BornList; //ボーンアドレス格納
	std::map<int, std::vector<int>> AnimationSetFrameNum; //各アニメーションのフレーム時間
	std::map<int, std::map<std::string, std::vector<D3DXMATRIX>>> AnimationSetMat; //各アニメーションの変換行列
};

class SkinMeshData
{
public:
	SkinMeshData();
	virtual ~SkinMeshData();
	SkinMeshInfo *GetSkinMeshInfo();
	BornInfo *GetBornInfo();
	virtual void Relese();
	virtual void ChangeAnimation(unsigned int num);
	virtual void SetRenewalTime(float animSpeed); //アニメーション速度更新
	virtual void SetTime(float animTime);		  //指定のアニメーション時間に設定
	virtual float GetPlayTime();
	virtual int GetPlayAnimation();
	virtual int GetBornAllNum();
	virtual std::string GetBornName(int bornIndex);
	virtual Matrix GetBornWorld(int bornIndex);
	virtual Vector3D GetBornPos(int bornIndex);
	virtual void Update();
	virtual void BornDebug(eBornDebug eBornDebug);
	virtual void AnimationDebug(int animNum);

protected:
	HRESULT InitShader();

protected:
	SkinMeshInfo m_MeshInfo;
	BornInfo m_BornInfo;

};

#endif