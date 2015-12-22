#include "StaticMeshAsset.h"
#include "..\System\Window.h"
#include <iostream>
#include <fstream>
#include <map>   

class StaticMeshAsset::StaticMeshPimpl
{
public:
	std::map<const std::string, MeshData*> m_Map;
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

MeshData *StaticMeshAsset::GetMesh(const std::string name)
{
	StaticMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	//指定のメッシュがない場合警告を出しウインドウ停止
	if (meshPimpl->m_Map.find(name) == meshPimpl->m_Map.end())
	{
		GetInstance()->AllClear();
		MessageBox(0, TEXT("指定のStaticMeshはありません"), NULL, MB_OK);
		Window::Get()->WindowEnd();
	}

	return meshPimpl->m_Map[name];
}

bool StaticMeshAsset::GetIsExistence(const std::string name)
{
	StaticMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	auto it = meshPimpl->m_Map.find(name);
	if (it != meshPimpl->m_Map.end()) return true;
	return false;
}

void StaticMeshAsset::CopyMesh(std::string copyName, const std::string name)
{
	StaticMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	//すでにある場合は警告を出しウィンドウ終了
	if (meshPimpl->m_Map.find(name) != meshPimpl->m_Map.end())
	{
		GetInstance()->AllClear();
		MessageBox(0, TEXT("すでに同じ名前が存在します"), NULL, MB_OK);
		Window::Get()->WindowEnd();
	}

	//すでにある場合は警告を出しウィンドウ終了
	if (meshPimpl->m_Map.find(copyName) == meshPimpl->m_Map.end())
	{
		GetInstance()->AllClear();
		MessageBox(0, TEXT("コピーする対象がありません"), NULL, MB_OK);
		Window::Get()->WindowEnd();
	}

	meshPimpl->m_Map[name] = meshPimpl->m_Map[copyName];
}

void StaticMeshAsset::LoadMesh(const std::string filmeName, const std::string name)
{
	StaticMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	//すでにある場合は警告を出しウィンドウ終了
	if (meshPimpl->m_Map.find(name) != meshPimpl->m_Map.end())
	{
		GetInstance()->AllClear();
		MessageBox(0, TEXT("指定のStaticMeshは存在します"), NULL, MB_OK);
		Window::Get()->WindowEnd();
	}

	//objファイルだったら読み込み
	if (filmeName[filmeName.length() - 1] == 'j' &&
		filmeName[filmeName.length() - 2] == 'b' &&
		filmeName[filmeName.length() - 3] == 'o')
	{
		meshPimpl->m_Map[name] = new MeshData();
		return;
	}

	//Xファイル判断だったら読み込み
	if (filmeName[filmeName.length() - 1] == 'x' &&
		filmeName[filmeName.length() - 2] == '.')
	{
		meshPimpl->m_Map[name] = new LoadXStatic(filmeName);
		return;
	}

	//ここまで来たらすべて削除して終了
	GetInstance()->AllClear();
	MessageBox(0, TEXT("StaticAssetが読み込みできるファイルではありません"), NULL, MB_OK);
	Window::Get()->WindowEnd();
}

void StaticMeshAsset::LoadFile(const std::string filmeName)
{
	std::ifstream ifs(filmeName);
	std::string str = filmeName;
	str += "読み込みに失敗しました";
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
	auto it = meshPimpl->m_Map.begin();
	for (; it != meshPimpl->m_Map.end(); it++)
	{
		std::cout << it->first << "は削除されました\n";
		it->second->Relese();
		delete it->second;
	}

	//全ての要素を削除
	meshPimpl->m_Map.clear();
}

void StaticMeshAsset::DebugDraw()
{
	StaticMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	//現在あるメッシュを表示
	auto it = meshPimpl->m_Map.begin();
	for (; it != meshPimpl->m_Map.end(); it++)
	{
		std::cout << it->first << "\n";
	}
}