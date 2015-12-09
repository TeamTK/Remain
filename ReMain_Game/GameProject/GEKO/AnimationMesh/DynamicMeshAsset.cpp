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

	//指定のメッシュがない場合警告を出しウインドウ停止
	if (meshPimpl->m_Map.find(name) == meshPimpl->m_Map.end())
	{
		GetInstance()->AllClear();
		MessageBox(0, L"指定のStaticMeshはありません", NULL, MB_OK);
		Window::Get()->WindowEnd();
	}

	return meshPimpl->m_Map[name];
}

void DynamicMeshAsset::CopyMesh(std::string copyName, std::string name)
{
	DynamicMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	//すでにある場合は警告を出しウィンドウ終了
	if (meshPimpl->m_Map.find(name) != meshPimpl->m_Map.end())
	{
		GetInstance()->AllClear();
		MessageBox(0, L"すでに同じ名前が存在します", NULL, MB_OK);
		Window::Get()->WindowEnd();
	}

	//すでにある場合は警告を出しウィンドウ終了
	if (meshPimpl->m_Map.find(copyName) == meshPimpl->m_Map.end())
	{
		GetInstance()->AllClear();
		MessageBox(0, L"コピーする対象がありません", NULL, MB_OK);
		Window::Get()->WindowEnd();
	}

	meshPimpl->m_Map[name] = meshPimpl->m_Map[copyName];
}

void DynamicMeshAsset::LoadMesh(std::string filmeName, std::string name)
{
	DynamicMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	//すでにある場合は警告を出しウィンドウ終了
	if (meshPimpl->m_Map.find(name) != meshPimpl->m_Map.end())
	{
		GetInstance()->AllClear();
		MessageBox(0, L"指定のDynamicMeshは存在します", NULL, MB_OK);
		Window::Get()->WindowEnd();
	}

	//Xファイル判断だったら読み込み
	if (filmeName[filmeName.length() - 1] == 'x' &&
		filmeName[filmeName.length() - 2] == '.')
	{
		meshPimpl->m_Map[name] = new LoadXDynamic(filmeName);
		return;
	}

	//ここまで来たらすべて削除して終了
	GetInstance()->AllClear();
	MessageBox(0, L"読み込みできるファイルではありません", NULL, MB_OK);
	Window::Get()->WindowEnd();
}

void DynamicMeshAsset::LoadFile(const std::string filmeName)
{
	std::ifstream ifs(filmeName);
	std::string str = filmeName;
	str += "読み込みに失敗しました";
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
		//it->second = nullptr;
		meshPimpl->m_Map.erase(it);
	}
}

void DynamicMeshAsset::AllClear()
{
	DynamicMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	//Meshのデータをリリース
	auto it = meshPimpl->m_Map.begin();
	for (; it != meshPimpl->m_Map.end(); it++)
	{
		std::cout << it->first << "\n";
		it->second->Relese();
		delete it->second;
		//it->second = nullptr;
	}

	//全ての要素を削除
	meshPimpl->m_Map.clear();
}

void DynamicMeshAsset::DebugDraw()
{
	DynamicMeshPimpl *meshPimpl = GetInstance()->m_pMeshPimpl;

	//現在あるメッシュを表示
	auto it = meshPimpl->m_Map.begin();
	for (; it != meshPimpl->m_Map.end(); it++)
	{
		std::cout << it->first << "\n";
		//std::cout << it->second.GetMeshInfo()->pvVertex[0].vPos.y << "\n";
	}
}