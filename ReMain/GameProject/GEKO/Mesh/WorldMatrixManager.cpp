#include "WorldMatrixManager.h"
#include <list>

//Pimpl�C�f�B�I���Ń��X�g�ێ�
class WorldMatrixManager::ListPimpl
{
public:
	std::list<WorldMatrixInfo*> m_List;
};

//���[���h�s��Ǘ��N���X
WorldMatrixManager::WorldMatrixManager()
{
	m_pListPimpl = new ListPimpl;
}

WorldMatrixManager::~WorldMatrixManager()
{
	m_pListPimpl->m_List.clear();
	delete m_pListPimpl;
}

WorldMatrixManager *WorldMatrixManager::GetInstance()
{
	static WorldMatrixManager WorldMatrixManager;
	return &WorldMatrixManager;
}

void WorldMatrixManager::Add(WorldMatrixInfo *worldMatrixInfo)
{
	m_pListPimpl->m_List.push_front(worldMatrixInfo);
}

void WorldMatrixManager::Update()
{
	D3DXQUATERNION qOut(0, 0, 0, 1); //�P�ʃN�H�[�^�j�I��
	D3DXQUATERNION qX(0, 0, 0, 1); //�P�ʃN�H�[�^�j�I��
	D3DXQUATERNION qY(0, 0, 0, 1); //�P�ʃN�H�[�^�j�I��
	D3DXQUATERNION qZ(0, 0, 0, 1); //�P�ʃN�H�[�^�j�I��
	D3DXVECTOR3 xAxis(1, 0, 0); //X�̒��S��
	D3DXVECTOR3 yAxis(0, 1, 0); //Y�̒��S��
	D3DXVECTOR3 zAxis(0, 0, 1); //Z�̒��S��
	D3DXMATRIX Mat;

	auto it = m_pListPimpl->m_List.begin();
	for (; it != m_pListPimpl->m_List.end(); it++)
	{
		D3DXQuaternionRotationAxis(&qX, &xAxis, (*it)->pRotation->x);
		D3DXQuaternionRotationAxis(&qY, &yAxis, (*it)->pRotation->y);
		D3DXQuaternionRotationAxis(&qZ, &zAxis, (*it)->pRotation->z);
		qOut = qX * qY * qZ;

		//�N�I�[�^�j�I������s��ɕύX
		D3DXMatrixRotationQuaternion(&Mat, &qOut);

		//�g��k��
		Mat._11 *= (*it)->pScale->x;
		Mat._21 *= (*it)->pScale->x;
		Mat._31 *= (*it)->pScale->x;

		Mat._12 *= (*it)->pScale->y;
		Mat._22 *= (*it)->pScale->y;
		Mat._32 *= (*it)->pScale->y;

		Mat._13 *= (*it)->pScale->z;
		Mat._23 *= (*it)->pScale->z;
		Mat._33 *= (*it)->pScale->z;

		//���s�ړ�
		Mat._41 = (*it)->pTranselate->x;
		Mat._42 = (*it)->pTranselate->y;
		Mat._43 = (*it)->pTranselate->z;

		*(*it)->pMatrix = Mat;
	}
}

void WorldMatrixManager::Clear()
{
	auto it = m_pListPimpl->m_List.begin();
	for (; it != m_pListPimpl->m_List.end(); it++)
	{
		//���[���h�s�񂪂Ȃ������烊�X�g���珜�O
		if ((*it)->pMatrix == nullptr)
		{
			it = m_pListPimpl->m_List.erase(it);
			break;
		}
	}
}

void WorldMatrixManager::AllClear()
{
	m_pListPimpl->m_List.clear();
}