#ifndef _STAGE_OBJECT_
#define _STAGE_OBJECT_
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
	int m_num;	//���v�I�u�W�F�N�g��
	int m_number;	//�e�I�u�W�F�N�g�̐�
	std::vector<std::string> m_ClsName;	//�N���X��
	std::vector<std::string> m_MdlName;	//���f����
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

//��1
class Tree_1 : public StageObject
{
public:
	Tree_1(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos,rot,sca,name)
	{
		m_CharacterHit.Regist_SMesh_vs_S(&m_Object);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2);
		m_BulletHit.Regist_SMesh_vs_L(&m_Object);
		m_BulletHit.SetID(eHITID3, eHITID2);
	}
	~Tree_1(){}

private:
	Collider m_BulletHit;
};

//��2
class Tree_2 : public StageObject
{
public:
	Tree_2(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name)
	{
		m_CharacterHit.Regist_SMesh_vs_S(&m_Object);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2);
		m_BulletHit.Regist_SMesh_vs_L(&m_Object);
		m_BulletHit.SetID(eHITID4, eHITID2);
	}
	~Tree_2(){}

private:
	Collider m_BulletHit;
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
		StageObject(pos, rot, sca, name)
	{
		//�����蔻��p���b�V��
		m_HitMesh.SetAsset("Cabin_Collision");
		m_HitMesh.SetTranselate(-7.5f, -0.08f, 12.6f);
		m_HitMesh.SetRotationDegree(0, 90, 0);

		//�L�����N�^�̓����蔻��
		m_CharacterHit.Regist_SMesh_vs_S(&m_HitMesh);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2);

		//�e�̓����蔻��
		m_BulletHit.Regist_SMesh_vs_L(&m_Object);
		m_BulletHit.SetID(eHITID5, eHITID2);
	}
	~Cabin(){}

private:
	StaticMesh m_HitMesh;
	Collider m_BulletHit;
};

class Cabin_Collision : public StageObject
{
public:
	Cabin_Collision(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) {}
	~Cabin_Collision() {}
};

class Ground : public StageObject
{
public:
	Ground(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) 
	{
		m_CharacterHit.Regist_SMesh_vs_S(&m_Object);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2 | eHITID3);

		m_CamraHit.Regist_SMesh_vs_L(&m_Object);
		m_CamraHit.SetID(eHITID1, eHITID0);

		m_BulletHit.Regist_SMesh_vs_L(&m_Object);
		m_BulletHit.SetID(eHITID6, eHITID2);
	}
	~Ground() {}

private:
	Collider m_CamraHit;
	Collider m_BulletHit;
};

class RockWall : public StageObject
{
public:
	RockWall(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) 
	{
		m_CharacterHit.Regist_SMesh_vs_S(&m_Object);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2 | eHITID3);

		m_CamraHit.Regist_SMesh_vs_L(&m_Object);
		m_CamraHit.SetID(eHITID1, eHITID0);

		m_BulletHit.Regist_SMesh_vs_L(&m_Object);
		m_BulletHit.SetID(eHITID7, eHITID2);
	}
	~RockWall() {}

private:
	Collider m_CamraHit;
	Collider m_BulletHit;
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

