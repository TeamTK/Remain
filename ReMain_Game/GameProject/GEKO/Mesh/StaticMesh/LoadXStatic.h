#ifndef _LOADXSTATIC_H_
#define _LOADXSTATIC_H_

#include "..\MeshData.h"

//StaticMesh�^�pX�t�@�C���ǂݍ���
class LoadXStatic : public StaticMeshData
{
public:
	LoadXStatic(std::string fileName);
	~LoadXStatic();
	void Relese();

private:
	HRESULT LoadXMesh(std::string fileName);

};

#endif