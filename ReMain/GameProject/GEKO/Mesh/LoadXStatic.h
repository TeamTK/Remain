#ifndef _LOADXSTATIC_H_
#define _LOADXSTATIC_H_

#include "MeshData.h"

struct MaterialX
{
	D3DXVECTOR3 Ambient;	//�A���r�G���g
	D3DXVECTOR4 Diffuse;	//�f�B�t���[�Y
	D3DXVECTOR3 Specular;	//�X�y�L�����[
	std::string TextureName;//�e�N�X�`���[�t�@�C����

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