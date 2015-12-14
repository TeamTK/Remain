#ifndef _DYNAMICMESH_ASSET_H_
#define _DYNAMICMESH_ASSET_H_

#include "LoadXDynamic.h"

class DynamicMeshAsset
{
public:
	~DynamicMeshAsset();
	static SkinMeshData* GetMesh(std::string name);
	static void CopyMesh(std::string copyName, std::string name);
	static void LoadMesh(std::string filmeName, std::string name);
	static void LoadFile(std::string filmeName);
	static void PartClear(std::string name);
	static void AllClear();
	static void DebugDraw();

private:
	DynamicMeshAsset();
	static DynamicMeshAsset* GetInstance();

private:
	class DynamicMeshPimpl;
	DynamicMeshPimpl *m_pMeshPimpl;
};

#endif