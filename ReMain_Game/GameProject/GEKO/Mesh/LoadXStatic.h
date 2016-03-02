#ifndef _LOADXSTATIC_H_
#define _LOADXSTATIC_H_

#include "MeshData.h"

struct MaterialX
{
	Vector3D ambient;	//アンビエント
	Vector4D diffuse;	//ディフューズ
	Vector3D specular;	//スペキュラー
	std::string textureName;//テクスチャーファイル名

	MaterialX(Vector4D &Diffuse, Vector3D &Specular, Vector3D &Ambient, const std::string &textureName) :
		diffuse(Diffuse),
		specular(Specular),
		ambient(Ambient),
		textureName(textureName)
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