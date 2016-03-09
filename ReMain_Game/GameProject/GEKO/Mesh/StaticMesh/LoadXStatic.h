#ifndef _LOADXSTATIC_H_
#define _LOADXSTATIC_H_

#include "..\MeshData.h"

//StaticMesh型用Xファイル読み込み
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