#include "DynamicMeshAsset.h"
#include "..\System\Window.h"
#include <iostream>
#include <map>   

class DynamicMeshAsset::DynamicMeshPimpl
{
public:
	std::map<std::string, SkinMeshData*> m_Map;
};

DynamicMeshAsset::DynamicMeshAsset()
{
	m_pMeshPimpl = new DynamicMeshPimpl;
}

DynamicMeshAsset::~DynamicMeshAsset()
{
	delete m_pMeshPimpl;
}

DynamicMeshAsset *DynamicMeshAsset::GetInstance()
{
	static DynamicMeshAsset DynamicMeshAsset;
	return &DynamicMeshAsset;
}

SkinMeshData *DynamicMeshAsset::GetMesh(std::string name)
{
	DynamicMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	//�w��̃��b�V�����Ȃ��ꍇ�x�����o���E�C���h�E��~
	if (meshPimpl->m_Map.find(name) == meshPimpl->m_Map.end())
	{
		GetInstance()->AllClear();
		MessageBox(0, L"�w���StaticMesh�͂���܂���", NULL, MB_OK);
		Window::Get()->WindowEnd();
	}

	return meshPimpl->m_Map[name];
}

void DynamicMeshAsset::CopyMesh(std::string copyName, std::string name)
{
	DynamicMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	//���łɂ���ꍇ�͌x�����o���E�B���h�E�I��
	if (meshPimpl->m_Map.find(name) != meshPimpl->m_Map.end())
	{
		GetInstance()->AllClear();
		MessageBox(0, L"���łɓ������O�����݂��܂�", NULL, MB_OK);
		Window::Get()->WindowEnd();
	}

	//���łɂ���ꍇ�͌x�����o���E�B���h�E�I��
	if (meshPimpl->m_Map.find(copyName) == meshPimpl->m_Map.end())
	{
		GetInstance()->AllClear();
		MessageBox(0, L"�R�s�[����Ώۂ�����܂���", NULL, MB_OK);
		Window::Get()->WindowEnd();
	}

	meshPimpl->m_Map[name] = meshPimpl->m_Map[copyName];
}

void DynamicMeshAsset::LoadMesh(std::string filmeName, std::string name)
{
	DynamicMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	//���łɂ���ꍇ�͌x�����o���E�B���h�E�I��
	if (meshPimpl->m_Map.find(name) != meshPimpl->m_Map.end())
	{
		GetInstance()->AllClear();
		MessageBox(0, L"�w���DynamicMesh�͑��݂��܂�", NULL, MB_OK);
		Window::Get()->WindowEnd();
	}

	//X�t�@�C�����f��������ǂݍ���
	if (filmeName[filmeName.length() - 1] == 'x' &&
		filmeName[filmeName.length() - 2] == '.')
	{
		meshPimpl->m_Map[name] = new LoadXDynamic(filmeName);
		return;
	}

	//�����܂ŗ����炷�ׂč폜���ďI��
	GetInstance()->AllClear();
	MessageBox(0, L"�ǂݍ��݂ł���t�@�C���ł͂���܂���", NULL, MB_OK);
	Window::Get()->WindowEnd();
}

void DynamicMeshAsset::LoadFile(const std::string filmeName)
{
	std::ifstream ifs(filmeName);
	std::string str = filmeName;
	str += "�ǂݍ��݂Ɏ��s���܂���";
	if (ifs.fail()) MessageBoxA(0, str.c_str(), NULL, MB_OK);

	while (!ifs.eof())
	{
		ifs >> str;

		if (str == "DynamicMesh")
		{
			ifs >> str; std::string SkinfileName = str;
			ifs >> str; std::string Name = str;
			DynamicMeshAsset::LoadMesh(SkinfileName, str);
		}
	}
}

void DynamicMeshAsset::PartClear(std::string name)
{
	DynamicMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	auto it = meshPimpl->m_Map.find(name);
	if (it != meshPimpl->m_Map.end())
	{
		std::cout << it->first << "�͍폜����܂���" << "\n";
		it->second->Relese();
		delete it->second;
		//it->second = nullptr;
		meshPimpl->m_Map.erase(it);
	}
}

void DynamicMeshAsset::AllClear()
{
	DynamicMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	//Mesh�̃f�[�^�������[�X
	auto it = meshPimpl->m_Map.begin();
	for (; it != meshPimpl->m_Map.end(); it++)
	{
		std::cout << it->first << "\n";
		it->second->Relese();
		delete it->second;
		//it->second = nullptr;
	}

	//�S�Ă̗v�f���폜
	meshPimpl->m_Map.clear();
}

void DynamicMeshAsset::DebugDraw()
{
	DynamicMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	//���݂��郁�b�V����\��
	auto it = meshPimpl->m_Map.begin();
	for (; it != meshPimpl->m_Map.end(); it++)
	{
		std::cout << it->first << "\n";
		//std::cout << it->second.GetMeshInfo()->pvVertex[0].vPos.y << "\n";
	}
}