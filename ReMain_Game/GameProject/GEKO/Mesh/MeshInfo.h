#ifndef _MESH_INFO_H_
#define _MESH_INFO_H_

#include "..\System\Math.h"
#include <string>
#include <assert.h>
#include <vector>
#include <fstream>
#include <map>

//レンダリングの種類
enum RenderingType
{
	eForward,
	eDeferred,
};

//3Dモデルの状態
enum MeshState
{
	eNothing = (1 << 0),
	eBlockingLight = (1 << 1),
	eShadow = (1 << 2),
	eFog = (1 << 3),
};

//3Dモデル行列の種類
enum MatrixType
{
	eLocalMatrix,
	eWorldMatrix,
	eModelMatrix,
};

//マテリアル構造体
struct MaterialInfo
{
	ID3D11ShaderResourceView* pTexture; //モデルのテクスチャ
	Vector4D diffuse;					//ディフューズ
	Vector3D specular;					//スペキュラー
	Vector3D ambient;					//アンビエント
	std::string textureName;			//テクスチャーファイル名
	int numPolygon;						//そのマテリアルであるポリゴン数
	MaterialInfo() :
		pTexture(nullptr),
		numPolygon(0) {}
	~MaterialInfo() {}
};

//頂点の構造体
struct VertexInfo
{
	Vector3D pos;    //頂点位置
	Vector3D normal; //頂点の法線
	Vector2D uv;	 //頂点のテクスチャの位置
};

//頂点のインデックス
struct IndexInfo
{
	int vertexIndex[3]; //ポリゴンの頂点のインデックス番号
	int materialIndex;  //ポリゴンのマテリアルインデックス番号
};

//メッシュの情報
struct MeshInfo
{
	ID3D11Buffer*		 pVertexBuffer;	 //頂点バッファー
	ID3D11Buffer**		 ppIndexBuffer;	 //インデックスバッファー
	ID3D11SamplerState*  pSampleLinear;	 //テクスチャーのサンプラー
	MaterialInfo*		 pMaterial;		 //マテリアル情報
	bool isTexture;						 //テクスチャー判断
	IndexInfo* pIndex;					 //全てのポリゴンのインデックス
	Matrix localMatrix;					 //モデルのローカル行列
	int vertexNumAll;					 //頂点数
	int faceNumAll;						 //面の数
	int materialNumAll;					 //マテリアルの数
	MeshInfo() :
		isTexture(true),
		vertexNumAll(0),
		faceNumAll(0),
		materialNumAll(0) {}
	~MeshInfo() {}
};

/******************************************/
//スキンメッシュアニメーション用データ
/******************************************/

//頂点の構造体
struct SkinVertexInfo
{
	Vector3D pos;
	Vector3D normal;
	Vector2D uv;
	UINT boneIndex[4];	 //ボーン番号
	float boneWeight[4];//ボーン重み
	SkinVertexInfo()
	{
		boneIndex[0] = 0;
		boneIndex[1] = 0;
		boneIndex[2] = 0;
		boneIndex[3] = 0;

		boneWeight[0] = 0.0f;
		boneWeight[1] = 0.0f;
		boneWeight[2] = 0.0f;
		boneWeight[3] = 0.0f;
	}
};

//ボーンのデバック種類
enum eBoneDebug
{
	eInitMat,
	eOffsetMat,
	eWorld,
	eBoneMat
};

//ボーン階層構造
struct Bone
{
	int indexId;
	Bone *brother;
	Bone *child;
	std::string boneName;
	Matrix initMat;
	Matrix offsetMat;
	Bone() :
		indexId(0),
		brother(nullptr),
		child(nullptr),
		boneName("NoName") {}
};

//コピーするボーン
struct CopyBone
{
	CopyBone *brother;
	CopyBone *child;
	Matrix worldMat;
	Matrix boneMat;
	Matrix parentAndChildMat;
	CopyBone() :
		brother(nullptr),
		child(nullptr) {}
};

//ボーン情報
struct BoneInfo
{
	Bone bone;	//ボーン階層構造
	std::vector<Bone *> BoneList; //ボーンアドレス格納
	std::map<int, std::vector<int>> AnimationSetFrameNum; //各アニメーションのフレーム時間
	std::map<int, std::map<std::string, std::vector<Matrix>>> AnimationSetMat; //各アニメーションの変換行列
};

#endif