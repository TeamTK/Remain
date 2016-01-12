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
	int m_num;	//合計オブジェクト数
	int m_number;	//各オブジェクトの数
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

//木1
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
		m_BulletHit.SetName("Tree_1");
	}
	~Tree_1(){}

private:
	Collider m_BulletHit;
};

//木2
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
		m_BulletHit.SetName("Tree_2");
	}
	~Tree_2(){}

private:
	Collider m_BulletHit;
};

//草1
class Grass_1 : public StageObject
{
public:
	Grass_1(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name){}
	~Grass_1(){}
};

//草2
class Grass_2 : public StageObject
{
public:
	Grass_2(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name){}
	~Grass_2(){}
};

//家
class Cabin : public StageObject
{
public:
	Cabin(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name)
	{
		//当たり判定用メッシュ
		m_HitMesh.SetAsset("Cabin_Collision");
		m_HitMesh.SetTranselate(pos.x, pos.y, pos.z);
		m_HitMesh.SetRotationDegree((int)rot.x, (int)rot.y, (int)rot.z);
		m_HitMesh.SetScale(sca.x, sca.y, sca.z);

		//キャラクタと家の当たり判定
		m_CharacterHit.Regist_SMesh_vs_S(&m_HitMesh);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2);

		//弾の当たり判定
		m_BulletHit.Regist_SMesh_vs_L(&m_HitMesh);
		m_BulletHit.SetID(eHITID5, eHITID2);
		m_BulletHit.SetName("Cabin");

		m_CameraHIt.Regist_SMesh_vs_L(&m_HitMesh);
		m_CameraHIt.SetID(eHITID1, eHITID0);
	}
	~Cabin(){}

private:
	StaticMesh m_HitMesh;
	Collider m_BulletHit;
	Collider m_CameraHIt;
};

//地面
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
		m_BulletHit.SetName("Ground");

	}
	~Ground() {}

private:
	Collider m_CamraHit;
	Collider m_BulletHit;
};

//壁
class RockWall : public StageObject
{
public:
	RockWall(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) 
	{
		//当たり判定用のメッシュ初期化
		m_HitMesh.SetAsset("Wall_Collision");
		m_HitMesh.SetTranselate(pos.x, pos.y, pos.z);
		m_HitMesh.SetRotationDegree((int)rot.x, (int)rot.y, (int)rot.z);
		m_HitMesh.SetScale(sca.x, sca.y, sca.z);

		//キャラクタと壁の当たり判定
		m_CharacterHit.Regist_SMesh_vs_S(&m_HitMesh);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2 | eHITID3);

		//カメラと壁の当たり判定
		m_CamraHit.Regist_SMesh_vs_L(&m_HitMesh);
		m_CamraHit.SetID(eHITID1, eHITID0);

		//弾と壁の当たり判定
		m_BulletHit.Regist_SMesh_vs_L(&m_HitMesh);
		m_BulletHit.SetID(eHITID7, eHITID2);
		m_BulletHit.SetName("RockWall");
	}
	~RockWall() {}

private:
	StaticMesh m_HitMesh;
	Collider m_CamraHit;
	Collider m_BulletHit;
};

//空
class SkyDome : public StageObject
{
public:
	SkyDome(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) {}
	~SkyDome() {}
};

//茂み
class Bush : public StageObject
{
public:
	Bush(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) {}
	~Bush() {}
};

//シダ
class Fern : public StageObject
{
public:
	Fern(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) {}
	~Fern() {}
};
#endif

