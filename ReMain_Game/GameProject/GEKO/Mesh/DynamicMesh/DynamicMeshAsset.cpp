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

	//指定のメッシュがない場合警告を出し
	if (meshPimpl->m_Map.find(name) == meshPimpl->m_Map.end())
	{
		std::string str = name + "はDynamicMeshAssetにはありません";
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

	//すでにある場合は警告を出し
	if (meshPimpl->m_Map.find(name) != meshPimpl->m_Map.end())
	{
		std::string str = name + "はDynamicMeshAssetに既に存在します";
		MessageBoxA(0, str.c_str(), NULL, MB_OK);
		return;
	}
	else
	{
		//Xファイル判断だったら読み込み
		if (filmeName[filmeName.length() - 1] == 'x' &&
			filmeName[filmeName.length() - 2] == '.')
		{
			meshPimpl->m_Map[name] = new LoadXDynamic(filmeName);
			return;
		}
	}

	//ここまで来たらすべて削除して終了
	std::string str = name + "はDynamicMeshAssetが読み込みできるファイルではありません";
	MessageBoxA(0, str.c_str(), NULL, MB_OK);
	Window::Get()->WindowEnd();
}

void DynamicMeshAsset::LoadFile(const std::string filmeName)
{
	std::ifstream ifs(filmeName);
	std::string str = filmeName;
	str += "の読み込みに失敗しました";
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
		std::cout << it->first << "は削除されました" << "\n";
		it->second->Relese();
		delete it->second;
		meshPimpl->m_Map.erase(it);
	}
}

void DynamicMeshAsset::AllClear()
{
	DynamicMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	//Meshのデータをリリース
	for (auto& i : meshPimpl->m_Map)
	{
		std::cout << i.first <<  "は削除されました"  << "\n";
		i.second->Relese();
		delete i.second;
	}

	//全ての要素を削除
	meshPimpl->m_Map.clear();
}

void DynamicMeshAsset::DebugDraw()
{
	DynamicMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	//現在あるメッシュを表示
	for (auto& i : meshPimpl->m_Map)
	{
		std::cout << i.first << "\n";
	}
}