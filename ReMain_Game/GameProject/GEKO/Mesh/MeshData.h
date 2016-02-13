#ifndef _MESH_DATA_H_
#define _MESH_DATA_H_

#include "..\System\Math.h"
#include <string>
#include <assert.h>
#include <vector>

//マテリアル構造体
struct MaterialInfo
{
	ID3D11ShaderResourceView* pTexture;
	Vector4D Diffuse;					//ディフューズ
	Vector3D Specular;					//スペキュラー
	Vector3D Ambient;					//アンビエント
	std::string TextureName;			//テクスチャーファイル名
	int dwNumFace;						//そのマテリアルであるポリゴン数
	MaterialInfo() :
		pTexture(nullptr),
		dwNumFace(0) {}
	~MaterialInfo() {}
};

//頂点の構造体
struct VertexData
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 uv;
};

//頂点の構造体
struct VertexInfo
{
	Vector3D pos;
	Vector3D normal;
	Vector2D uv;
};

//頂点のインデックス
struct IndexInfo
{
	int vertexIndex[3];
	int materialIndex;
};

//メッシュの情報
struct MeshInfo
{
	ID3D11Buffer*		 m_pVertexBuffer;	 //頂点バッファー
	ID3D11Buffer**		 m_ppIndexBuffer;	 //インデックスバッファー
	ID3D11SamplerState*  m_pSampleLinear;	 //テクスチャーのサンプラー
	MaterialInfo*		 m_pMaterial;		 //マテリアル情報
	bool m_IsTexture;						 //テクスチャー判断
	VertexInfo* pVertex;					 //頂点情報
	IndexInfo* pIndex;
	Matrix m_LocalMat;						 //モデルのローカル行列
	int vertexNumAll;						 //頂点数
	int faceNumAll;							 //面の数
	int normalNumAll;						 //法線の数
	int materialNumAll;						 //マテリアルの数
	int materialListNumAll;					 //マテリアルリストの数;
	int uvNumAll;							 //UVの数
	MeshInfo() :
		m_IsTexture(true),
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
	MeshInfo m_MeshInfo;

private:

};

#endif 