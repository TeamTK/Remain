#ifndef _STATICMESHASSET_H_
#define _STATICMESHASSET_H_

#include "LoadXStatic.h"

class StaticMeshAsset
{
public:
	~StaticMeshAsset();
	static MeshData* GetMesh(const std::string name);
	static bool GetIsExistence(const std::string name);
	static void LoadMesh(const std::string filmeName, const std::string name);
	static void LoadFile(const std::string filmeName);
	static void PartClear(const std::string name);
	static void AllClear();
	static void DebugDraw();

private:
	StaticMeshAsset();
	static StaticMeshAsset* GetInstance();

private:
	class StaticMeshPimpl;
	StaticMeshPimpl *m_pMeshPimpl;
};

#endif