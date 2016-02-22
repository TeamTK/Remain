#ifndef _STAGE_OBJECT_
#define _STAGE_OBJECT_

#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include "../../GEKO/Collider/Collider.h"
#include "../../GEKO/Mesh/StaticMesh.h"

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
	StageObject(XYZ pos, XYZ rot, XYZ sca, std::string name, bool isShadow = false, bool isLightInterrupted = false);
	virtual ~StageObject();
	void Update();
	void Render();

protected:
	bool m_IsShadow;
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
		StageObject(pos,rot,sca,name, true, true)
	{
		m_CharacterHit.Regist_SMesh_vs_S(&m_Object);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2);

		m_BulletHit.Regist_SMesh_vs_L(&m_Object, true);
		m_BulletHit.SetID(eHITID4, eHITID2);
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
		StageObject(pos, rot, sca, name, true, true)
	{
		m_CharacterHit.Regist_SMesh_vs_S(&m_Object);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2);

		m_BulletHit.Regist_SMesh_vs_L(&m_Object, true);
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
		StageObject(pos, rot, sca, name, false, true){}
	~Grass_1(){}
};

//草2
class Grass_2 : public StageObject
{
public:
	Grass_2(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name, false, true){}
	~Grass_2(){}
};

//家
class Cabin : public StageObject
{
public:
	Cabin(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name, true, true)
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
		m_BulletHit.Regist_SMesh_vs_L(&m_HitMesh, true);
		m_BulletHit.SetID(eHITID5, eHITID2);
		m_BulletHit.SetName("Cabin");

		//カメラとの当たり判定
		m_CameraHIt.Regist_SMesh_vs_L(&m_HitMesh, true);
		m_CameraHIt.SetID(eHITID3, eHITID2);
	}
	~Cabin(){}

private:
	StaticMesh m_HitMesh;
	Collider m_BulletHit;
	Collider m_CameraHIt;
};

//地面_1_1
class Ground_1_1 : public StageObject
{
public:
	Ground_1_1(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name, true) 
	{
		m_CharacterHit.Regist_SMesh_vs_S(&m_Object);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2 | eHITID3);

		m_CameraHit.Regist_SMesh_vs_L(&m_Object, true);
		m_CameraHit.SetID(eHITID1, eHITID0);

		m_BulletHit.Regist_SMesh_vs_L(&m_Object, true);
		m_BulletHit.SetID(eHITID6, eHITID2);
		m_BulletHit.SetName("Ground_1_1");
	}
	~Ground_1_1() {}

private:
	Collider m_CameraHit;
	Collider m_BulletHit;
};
//地面_1_2
class Ground_1_2 : public StageObject
{
public:
	Ground_1_2(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name, true)
	{
		m_CharacterHit.Regist_SMesh_vs_S(&m_Object);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2 | eHITID3);

		m_CameraHit.Regist_SMesh_vs_L(&m_Object, true);
		m_CameraHit.SetID(eHITID1, eHITID0);

		m_BulletHit.Regist_SMesh_vs_L(&m_Object, true);
		m_BulletHit.SetID(eHITID6, eHITID2);
		m_BulletHit.SetName("Ground_1_2");
	}
	~Ground_1_2() {}

private:
	Collider m_CameraHit;
	Collider m_BulletHit;
};

//壁_1_1
class RockWall_1_1 : public StageObject
{
public:
	RockWall_1_1(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name, true, true) 
	{
		//当たり判定用のメッシュ初期化
		//m_HitMesh.SetAsset("RockWall");
		m_HitMesh.SetAsset("Wall_Collision_1_1");
		m_HitMesh.SetTranselate(pos.x, pos.y, pos.z);
		m_HitMesh.SetRotationDegree((int)rot.x, (int)rot.y, (int)rot.z);
		m_HitMesh.SetScale(sca.x, sca.y, sca.z);

		//キャラクタと壁の当たり判定
		m_CharacterHit.Regist_SMesh_vs_S(&m_HitMesh);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2 | eHITID3);

		//カメラと壁の当たり判定
		m_CameraHit.Regist_SMesh_vs_L(&m_HitMesh, true);
		m_CameraHit.SetID(eHITID1, eHITID0);

		//弾と壁の当たり判定
		m_BulletHit.Regist_SMesh_vs_L(&m_Object, true);
		m_BulletHit.SetID(eHITID7, eHITID2);
		m_BulletHit.SetName("RockWall_1_1");
	}
	~RockWall_1_1() {}

private:
	StaticMesh m_HitMesh;
	Collider m_CameraHit;
	Collider m_BulletHit;
};

//壁_1_2
class RockWall_1_2 : public StageObject
{
public:
	RockWall_1_2(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name, true, true)
	{
		//当たり判定用のメッシュ初期化
		//m_HitMesh.SetAsset("RockWall");
		m_HitMesh.SetAsset("Wall_Collision_1_2");
		m_HitMesh.SetTranselate(pos.x, pos.y, pos.z);
		m_HitMesh.SetRotationDegree((int)rot.x, (int)rot.y, (int)rot.z);
		m_HitMesh.SetScale(sca.x, sca.y, sca.z);

		//キャラクタと壁の当たり判定
		m_CharacterHit.Regist_SMesh_vs_S(&m_HitMesh);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2 | eHITID3);

		//カメラと壁の当たり判定
		m_CameraHit.Regist_SMesh_vs_L(&m_HitMesh, true);
		m_CameraHit.SetID(eHITID1, eHITID0);

		//弾と壁の当たり判定
		m_BulletHit.Regist_SMesh_vs_L(&m_Object, true);
		m_BulletHit.SetID(eHITID7, eHITID2);
		m_BulletHit.SetName("RockWall_1_2");
	}
	~RockWall_1_2() {}

private:
	StaticMesh m_HitMesh;
	Collider m_CameraHit;
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
		StageObject(pos, rot, sca, name, true, false) {}
	~Bush() {}
};

//クローバー
class Clover : public StageObject
{
public:
	Clover(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name, true, false) {}
	~Clover() {}
};

//シダ
class Fern : public StageObject
{
public:
	Fern(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name, true, true) {}
	~Fern() {}
};

//フェンス
class Fence : public StageObject
{
public:
	Fence(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name, true, true)
	{
		m_HitMesh.SetAsset("Fence_Collision");
		m_HitMesh.SetTranselate(pos.x, pos.y, pos.z);
		m_HitMesh.SetRotationDegree((int)rot.x, (int)rot.y, (int)rot.z);
		m_HitMesh.SetScale(sca.x, sca.y, sca.z);

		//キャラクタと壁の当たり判定
		m_CharacterHit.Regist_SMesh_vs_S(&m_HitMesh);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2);
	}
	~Fence() {}
private:
	StaticMesh m_HitMesh;
};

//石A
class Stone_A : public StageObject
{
public:
	Stone_A(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name, true, true)
	{
		m_HitMesh.SetAsset("Stone_A_Collision");
		m_HitMesh.SetTranselate(pos.x, pos.y, pos.z);
		m_HitMesh.SetRotationDegree((int)rot.x, (int)rot.y, (int)rot.z);
		m_HitMesh.SetScale(sca.x, sca.y, sca.z);

		//キャラクタと壁の当たり判定
		m_CharacterHit.Regist_SMesh_vs_S(&m_HitMesh);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2 | eHITID3);

		//カメラと壁の当たり判定
		m_CameraHit.Regist_SMesh_vs_L(&m_HitMesh, true);
		m_CameraHit.SetID(eHITID1, eHITID0);
	}
	~Stone_A() {}
private:
	StaticMesh m_HitMesh;
	Collider m_CameraHit;
};

//落ち葉
class Fallen_leaves : public StageObject
{
public:
	Fallen_leaves(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name, true) {}
	~Fallen_leaves() {}
};

//枯れ木
class Tree_1_Trunk : public StageObject
{
public:
	Tree_1_Trunk(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name, false, true) {}
	~Tree_1_Trunk() {}
};

//低木
class Shrub : public StageObject
{
public:
	Shrub(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name, false, true) {}
	~Shrub() {}
};

//低木
class Weeds : public StageObject
{
public:
	Weeds(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name, false, true) {}
	~Weeds() {}
};

//木箱
class WoodBox : public StageObject
{
public:
	WoodBox(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name, true, true)
	{
		//プレイヤーの判定は仮
		m_CharacterHit.Regist_SMesh_vs_S(&m_Object);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2);
		m_BulletHit.Regist_SMesh_vs_L(&m_Object, true);
		m_BulletHit.SetID(eHITID3, eHITID2);
		m_BulletHit.SetName("WoodBox");
	}
	~WoodBox() {}

private:
	Collider m_BulletHit;
};
#endif

