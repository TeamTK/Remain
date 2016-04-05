#ifndef _WORLDMATRIXMANAGER_H_
#define _WORLDMATRIXMANAGER_H_

#include "..\System\Math.h"
#include <functional>

//���f���̍s����Ǘ�������
struct WorldMatrixInfo
{
	std::function<void()> func;
};

//���f���̃{�[���s����Ǘ�������
struct BoneMatrixFuncInfo
{
	std::function<void()> func;
};

//�S�Ă�3D���f���̍s����Ǘ�
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
	//�C�f�B�I���Ƀf�[�^���`
	class ListPimpl;
	ListPimpl *m_pListPimpl;
};

#endif