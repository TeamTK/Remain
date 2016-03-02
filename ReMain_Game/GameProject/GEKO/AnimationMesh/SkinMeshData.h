#ifndef _SKINMESH_DATA_H_
#define _SKINMESH_DATA_H_

#include "..\System\Math.h"
#include <string>
#include <vector>
#include <fstream>
#include <map>

#define MAX_BONES 255

//マテリアル構造体
struct SkinMaterialInfo
{
	ID3D11ShaderResourceView* pTexture;
	Vector4D diffuse;	//ディフューズ
	Vector3D specular;	//スペキュラー
	Vector3D ambient;	//アンビエント
	std::string textureName;//テクスチャーファイル名
	int numPolygon;		//そのマテリアルであるポリゴン数
	SkinMaterialInfo() :
		pTexture(nullptr),
		numPolygon(0) {}
	~SkinMaterialInfo() {}
};

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
	ID3D11Buffer*		 pVertexBuffer;	 //頂点バッファー
	ID3D11Buffer**		 ppIndexBuffer;	 //インデックスバッファー
	ID3D11SamplerState*  pSampleLinear;	 //テクスチャーのサンプラー
	SkinMaterialInfo*	 pMaterial;		 //マテリアル情報
	DWORD				 dwNumMaterial;	 //マテリアルの数
	bool isTexture;						 //テクスチャー判断
	SkinVertexInfo* pVertex;				 //頂点情報
	int vertexNumAll;						 //頂点数
	int faceNumAll;						 //面の数
	int normalNumAll;						 //法線の数
	int materialNumAll;					 //マテリアルの数
	int materialListNumAll;				 //マテリアルリストの数;
	int uvNumAll;							 //UVの数
	SkinMeshInfo() :
		isTexture(true),
		pVertexBuffer(nullptr),
		pMaterial(nullptr),
		pVertex(nullptr),
		faceNumAll(0),
		normalNumAll(0),
		materialNumAll(0),
		materialListNumAll(0),
		uvNumAll(0) {}
	~SkinMeshInfo() {}
};

//ボーン階層構造
struct Born
{
	int indexId;
	Born *brother;
	Born *child;
	std::string BornName;
	Matrix initMat;
	Matrix offsetMat;
	Born() :
		indexId(0),
		brother(nullptr),
		child(nullptr),
		BornName("NoName") {}
};

//コピーするボーン
struct CopyBorn
{
	CopyBorn *brother;
	CopyBorn *child;
	Matrix worldMat;
	Matrix bornMat;
	Matrix ParentAndChildMat;
	unsigned int animNum;
	float animFrame;
	CopyBorn() :
		brother(nullptr),
		child(nullptr),
		animNum(0),
		animFrame(0.0f) {}
};

//ボーン情報
struct BornInfo
{
	Born sBorn;	//ボーン階層構造
	std::vector<Born *> BornList; //ボーンアドレス格納
	std::map<int, std::vector<int>> AnimationSetFrameNum; //各アニメーションのフレーム時間
	std::map<int, std::map<std::string, std::vector<Matrix>>> AnimationSetMat; //各アニメーションの変換行列
};

//スキンメッシュデータ
class SkinMeshData
{
public:
	SkinMeshData();
	virtual ~SkinMeshData();
	const SkinMeshInfo *GetSkinMeshInfo() const;
	const BornInfo *GetBornInfo() const;
	virtual void Relese();
	virtual void Update(CopyBorn *pCopyBorn);
	virtual void CopyBornTree(CopyBorn *pBornCopy, std::vector<CopyBorn*> *pCopyBornArray, Born *pBornOriginal);

protected:
	HRESULT InitShader();

protected:
	SkinMeshInfo m_MeshInfo;
	BornInfo m_BornInfo;

};

#endif