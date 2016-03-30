#ifndef _WORLDMATRIXMANAGER_H_
#define _WORLDMATRIXMANAGER_H_

#include "..\System\Math.h"

//モデルの行列を管理する情報
struct WorldMatrixInfo
{
	Matrix *pLocalMatrix;
	Matrix *pWorldMatrix;
	Matrix *pModelMatrix;
	Vector3D *pRotation;
	Vector3D *pScale;
	Vector3D *pTranselate;
};

//全ての3Dモデルの行列を管理
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
	//イディオムにデータを定義
	class ListPimpl;
	ListPimpl *m_pListPimpl;
};

#endif