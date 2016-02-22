#ifndef _WORLDMATRIXMANAGER_H_
#define _WORLDMATRIXMANAGER_H_

#include "..\System\Math.h"

struct WorldMatrixInfo
{
	Matrix *pWorldMatrix;
	Vector3D *pRotation;
	Vector3D *pScale;
	Vector3D *pTranselate;
};

class WorldMatrixManager
{
public:
	~WorldMatrixManager();
	static WorldMatrixManager *GetInstance();
	void Add(WorldMatrixInfo *worldMatrixInfo);
	void Update();
	void Clear();
	void AllClear();

private:
	WorldMatrixManager();

private:
	class ListPimpl;
	ListPimpl *m_pListPimpl;
};

#endif