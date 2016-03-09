#include "DynamicMeshAsset.h"
#include "..\..\System\Window.h"
#include <iostream>
#include <map>   

class DynamicMeshAsset::DynamicMeshPimpl
{
public:
	std::map<std::string, DyanmicMeshData*> m_Map;
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

DyanmicMeshData *DynamicMeshAsset::GetMesh(std::string name)
{
	DynamicMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	//�w��̃��b�V�����Ȃ��ꍇ�x�����o��
	if (meshPimpl->m_Map.find(name) == meshPimpl->m_Map.end())
	{
		std::string str = name + "��DynamicMeshAsset�ɂ͂���܂���";
		MessageBoxA(0, str.c_str(), NULL, MB_OK);
	}
	else
	{
		return meshPimpl->m_Map[name];
	}
	return nullptr;
}

void DynamicMeshAsset::LoadMesh(std::string filmeName, std::string name)
{
	DynamicMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	//���łɂ���ꍇ�͌x�����o��
	if (meshPimpl->m_Map.find(name) != meshPimpl->m_Map.end())
	{
		std::string str = name + "��DynamicMeshAsset�Ɋ��ɑ��݂��܂�";
		MessageBoxA(0, str.c_str(), NULL, MB_OK);
		return;
	}
	else
	{
		//X�t�@�C�����f��������ǂݍ���
		if (filmeName[filmeName.length() - 1] == 'x' &&
			filmeName[filmeName.length() - 2] == '.')
		{
			meshPimpl->m_Map[name] = new LoadXDynamic(filmeName);
			return;
		}
	}

	//�����܂ŗ����炷�ׂč폜���ďI��
	std::string str = name + "��DynamicMeshAsset���ǂݍ��݂ł���t�@�C���ł͂���܂���";
	MessageBoxA(0, str.c_str(), NULL, MB_OK);
	Window::Get()->WindowEnd();
}

void DynamicMeshAsset::LoadFile(const std::string filmeName)
{
	std::ifstream ifs(filmeName);
	std::string str = filmeName;
	str += "�̓ǂݍ��݂Ɏ��s���܂���";
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
		meshPimpl->m_Map.erase(it);
	}
}

void DynamicMeshAsset::AllClear()
{
	DynamicMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	//Mesh�̃f�[�^�������[�X
	for (auto& i : meshPimpl->m_Map)
	{
		std::cout << i.first <<  "�͍폜����܂���"  << "\n";
		i.second->Relese();
		delete i.second;
	}

	//�S�Ă̗v�f���폜
	meshPimpl->m_Map.clear();
}

void DynamicMeshAsset::DebugDraw()
{
	DynamicMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	//���݂��郁�b�V����\��
	for (auto& i : meshPimpl->m_Map)
	{
		std::cout << i.first << "\n";
	}
}