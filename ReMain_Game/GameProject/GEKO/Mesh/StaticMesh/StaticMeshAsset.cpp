#include "StaticMeshAsset.h"
#include "..\..\System\Window.h"
#include <iostream>
#include <fstream>
#include <map>   

class StaticMeshAsset::StaticMeshPimpl
{
public:
	std::map<const std::string, StaticMeshData*> m_Map;
};

StaticMeshAsset::StaticMeshAsset()
{
	m_pMeshPimpl = new StaticMeshPimpl;
}

StaticMeshAsset::~StaticMeshAsset()
{
	delete m_pMeshPimpl;
}

StaticMeshAsset *StaticMeshAsset::GetInstance()
{
	static StaticMeshAsset staticMeshAsset;
	return &staticMeshAsset;
}

StaticMeshData *StaticMeshAsset::GetMesh(const std::string name)
{
	StaticMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	//指定のメッシュがない場合警告を出す
	if (meshPimpl->m_Map.find(name) == meshPimpl->m_Map.end())
	{
		std::string str = name + "はStaticMeshAssetにはありません";
		MessageBoxA(0, str.c_str(), NULL, MB_OK);
	}
	else
	{
		return meshPimpl->m_Map[name];
	}
	return nullptr;
}

bool StaticMeshAsset::GetIsExistence(const std::string name)
{
	StaticMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	auto it = meshPimpl->m_Map.find(name);
	if (it != meshPimpl->m_Map.end()) return true;
	return false;
}

void StaticMeshAsset::LoadMesh(const std::string filmeName, const std::string name)
{
	StaticMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	//すでにある場合は警告を出す
	if (meshPimpl->m_Map.find(name) != meshPimpl->m_Map.end())
	{
		std::string str = name + "はStaticMeshAssetに既に存在します";
		MessageBoxA(0, str.c_str(), NULL, MB_OK);
		return;
	}
	else
	{
		//objファイルだったら読み込み
		if (filmeName[filmeName.length() - 1] == 'j' &&
			filmeName[filmeName.length() - 2] == 'b' &&
			filmeName[filmeName.length() - 3] == 'o')
		{
			meshPimpl->m_Map[name] = new StaticMeshData();
			return;
		}

		//Xファイル判断だったら読み込み
		if (filmeName[filmeName.length() - 1] == 'x' &&
			filmeName[filmeName.length() - 2] == '.')
		{
			meshPimpl->m_Map[name] = new LoadXStatic(filmeName);
			return;
		}
	}

	//ここまで来たらすべて削除して終了
	std::string str = filmeName + "はStaticMeshAssetが読み込みできるファイルではありません";
	MessageBoxA(0, str.c_str(), NULL, MB_OK);
	Window::Get()->WindowEnd();
}

void StaticMeshAsset::LoadFile(const std::string filmeName)
{
	std::ifstream ifs(filmeName);
	std::string str = filmeName;
	str += "の読み込みに失敗しました";
	if (ifs.fail()) MessageBoxA(0, str.c_str(), NULL, MB_OK);

	while (!ifs.eof())
	{
		ifs >> str;

		if (str == "StaticMesh")
		{
			ifs >> str; std::string MeshfileName = str;
			ifs >> str; std::string Name = str;
			StaticMeshAsset::LoadMesh(MeshfileName, str);
		}
	}
}

void StaticMeshAsset::PartClear(const std::string name)
{	
	StaticMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	auto it = meshPimpl->m_Map.find(name);
	if (it != meshPimpl->m_Map.end())
	{
		std::cout << it->first << "は削除されました" << "\n";
		it->second->Relese();
		delete it->second;
		it->second = nullptr;
		meshPimpl->m_Map.erase(it);
	}
}

void StaticMeshAsset::AllClear()
{
	StaticMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	//Meshのデータをリリース
	for (auto& i : meshPimpl->m_Map)
	{
		std::cout << i.first << "は削除されました\n";
		i.second->Relese();
		delete i.second;
	}

	//全ての要素を削除
	meshPimpl->m_Map.clear();
}

void StaticMeshAsset::DebugDraw()
{
	StaticMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	//現在あるメッシュを表示
	for (auto& i : meshPimpl->m_Map)
	{
		std::cout << i.first << "\n";
	}
}