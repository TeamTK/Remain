#ifndef _WORLDMATRIXMANAGER_H_
#define _WORLDMATRIXMANAGER_H_

#include "..\System\Math.h"

//���f���̍s����Ǘ�������
struct WorldMatrixInfo
{
	Matrix *pLocalMatrix;
	Matrix *pWorldMatrix;
	Matrix *pModelMatrix;
	Vector3D *pRotation;
	Vector3D *pScale;
	Vector3D *pTranselate;
};

//�S�Ă�3D���f���̍s����Ǘ�
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
	//�C�f�B�I���Ƀf�[�^���`
	class ListPimpl;
	ListPimpl *m_pListPimpl;
};

#endif