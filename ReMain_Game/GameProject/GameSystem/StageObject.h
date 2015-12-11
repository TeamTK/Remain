#ifndef STAGE_OBJECT
#define STAGE_OBJECT
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include "../GEKO//Mesh/StaticMesh.h"
#include "../GEKO/Collider/Collider.h"

struct XYZ
{
	float x;
	float y;
	float z;

	XYZ(){};
	XYZ(float x, float y, float z) :
		x(x),
		y(y),
		z(z){}
};

class MapObject;

class StageObject
{
public:
	StageObject();
	StageObject(XYZ pos, XYZ rot, XYZ sca, std::string name);
	virtual ~StageObject();
	void Update();
	void Render();

protected:
	StaticMesh m_Object;
	Collider m_CharacterHit;

};

class StageObjectManager
{
public:
	StageObjectManager();
	~StageObjectManager();
	void LoadObject(char* filename);
	void Update();
	void Render();
	void ClearList();
	static StageObjectManager* GetInstance();
	static void ClearInstance();

private:
	int m_cnt;
	float m_number;	//各オブジェクトの数
	std::vector<std::string> m_ClsName;	//クラス名
	std::vector<std::string> m_MdlName;	//モデル名
	std::vector<XYZ> m_Position;
	std::vector<XYZ> m_Rotation;
	std::vector<XYZ> m_Scale;
	std::list<MapObject> m_MapObjectList;
	static StageObjectManager* m_Instance;
};

class MapObject
{
public:
	MapObject();
	MapObject(XYZ &pos, XYZ &rot, XYZ &sca, std::string name);
	~MapObject();
	void Render();
	void Relese();

private:
	StageObject *mp_StageObject;
};



class Tree_1 : public StageObject
{
public:
	Tree_1(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos,rot,sca,name)
	{
		m_CharacterHit.Regist_SMesh_vs_S(&m_Object);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2);
		m_CamraHit.Regist_SMesh_vs_L(&m_Object);
		m_CamraHit.SetID(eHITID1, eHITID0);
	}
	~Tree_1(){}

private:
	Collider m_CamraHit;
};

class Tree_2 : public StageObject
{
public:
	Tree_2(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name)
	{
		m_CharacterHit.Regist_SMesh_vs_S(&m_Object);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2);
		m_CamraHit.Regist_SMesh_vs_L(&m_Object);
		m_CamraHit.SetID(eHITID1, eHITID0);
	}
	~Tree_2(){}

private:
	Collider m_CamraHit;
};

class Grass_1 : public StageObject
{
public:
	Grass_1(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name){}
	~Grass_1(){}
};

class Grass_2 : public StageObject
{
public:
	Grass_2(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name){}
	~Grass_2(){}
};

class Cabin : public StageObject
{
public:
	Cabin(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name){}
	~Cabin(){}
};

class Ground : public StageObject
{
public:
	Ground(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) 
	{
		m_CharacterHit.Regist_SMesh_vs_S(&m_Object);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2);
		m_CamraHit.Regist_SMesh_vs_L(&m_Object);
		m_CamraHit.SetID(eHITID1, eHITID0);
	}
	~Ground() {}

private:
	Collider m_CamraHit;
};

class RockWall : public StageObject
{
public:
	RockWall(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) 
	{
		m_CharacterHit.Regist_SMesh_vs_S(&m_Object);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2);
		m_CamraHit.Regist_SMesh_vs_L(&m_Object);
		m_CamraHit.SetID(eHITID1, eHITID0);
	}
	~RockWall() {}

private:
	Collider m_CamraHit;
};

class SkyDome : public StageObject
{
public:
	SkyDome(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) {}
	~SkyDome() {}
};

class Bush : public StageObject
{
public:
	Bush(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) {}
	~Bush() {}
};

class Fern : public StageObject
{
public:
	Fern(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) {}
	~Fern() {}
};
#endif

