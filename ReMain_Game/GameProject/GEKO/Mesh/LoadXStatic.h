#ifndef _LOADXSTATIC_H_
#define _LOADXSTATIC_H_

#include "MeshData.h"

struct MaterialX
{
	Vector3D ambient;	//�A���r�G���g
	Vector4D diffuse;	//�f�B�t���[�Y
	Vector3D specular;	//�X�y�L�����[
	std::string textureName;//�e�N�X�`���[�t�@�C����

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