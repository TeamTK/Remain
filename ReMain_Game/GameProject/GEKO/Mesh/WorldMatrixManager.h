#ifndef _WORLDMATRIXMANAGER_H_
#define _WORLDMATRIXMANAGER_H_

#include "..\System\Math.h"
#include <functional>

//モデルの行列を管理する情報
struct WorldMatrixInfo
{
	std::function<void()> func;
};

//モデルのボーン行列を管理する情報
struct BoneMatrixFuncInfo
{
	std::function<void()> func;
};

//全ての3Dモデルの行列を管理
class WorldMatrixManager
{
public:
	~WorldMatrixManager();
	static WorldMatrixManager *GetInstance();
	void Add(WorldMatrixInfo *worldMatrixInfo);
	void Add(BoneMatrixFuncInfo *pBoneMatrixFuncInfo);
	void Update();
	void BoneUpdate();
	void Clear(WorldMatrixInfo *worldMatrixInfo);
	void Clear(BoneMatrixFuncInfo *pBoneMatrixFuncInfo);
	void AllClear();

private:
	WorldMatrixManager();

private:
	//イディオムにデータを定義
	class ListPimpl;
	ListPimpl *m_pListPimpl;
};

#endif