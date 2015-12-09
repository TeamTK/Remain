#ifndef _LOADXSTATIC_H_
#define _LOADXSTATIC_H_

#include "MeshData.h"

struct MaterialX
{
	D3DXVECTOR3 Ambient;	//アンビエント
	D3DXVECTOR4 Diffuse;	//ディフューズ
	D3DXVECTOR3 Specular;	//スペキュラー
	std::string TextureName;//テクスチャーファイル名

	MaterialX(D3DXVECTOR4 &Diffuse, D3DXVECTOR3 &Specular, D3DXVECTOR3 &Ambient, const std::string &TextureName) :
		Diffuse(Diffuse),
		Specular(Specular),
		Ambient(Ambient),
		TextureName(TextureName)
	{
	}
};

class LoadXStatic : public MeshData
{
public:
	LoadXStatic(std::string fileName);
	~LoadXStatic();
	void Relese();

private:
	HRESULT LoadXMesh(std::string fileName);

};

#endif