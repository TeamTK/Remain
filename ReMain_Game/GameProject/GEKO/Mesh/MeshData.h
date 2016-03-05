#ifndef _MESH_DATA_H_
#define _MESH_DATA_H_

#include "..\System\Math.h"
#include <string>
#include <assert.h>
#include <vector>

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
	VertexInfo* pVertex;			     //頂点情報
	IndexInfo* pIndex;					 //全てのポリゴンのインデックス
	Matrix localMat;					 //モデルのローカル行列
	int vertexNumAll;					 //頂点数
	int faceNumAll;						 //面の数
	int normalNumAll;					 //法線の数
	int materialNumAll;					 //マテリアルの数
	int materialListNumAll;				 //マテリアルリストの数;
	int uvNumAll;						 //UVの数
	MeshInfo() :
		isTexture(true),
		vertexNumAll(0),
		faceNumAll(0),
		normalNumAll(0),
		materialNumAll(0),
		materialListNumAll(0),
		uvNumAll(0) {}
	~MeshInfo() {}
};

//メッシュデータ
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
	MeshInfo m_MeshInfo; //メッシュの情報

private:

};

#endif 