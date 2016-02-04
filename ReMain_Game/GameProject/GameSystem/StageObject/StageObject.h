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
		m_BulletHit.Regist_SMesh_vs_L(&m_Object, true);
		m_BulletHit.SetID(eHITID4, eHITID2);
		m_BulletHit.SetName("Tree_1");
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
		m_BulletHit.Regist_SMesh_vs_L(&m_Object, true);
		m_BulletHit.SetID(eHITID4, eHITID2);
		m_BulletHit.SetName("Tree_2");
	}
	~Tree_2(){}

private:
	Collider m_BulletHit;
};

//��1
class Grass_1 : public StageObject
{
public:
	Grass_1(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name){}
	~Grass_1(){}
};

//��2
class Grass_2 : public StageObject
{
public:
	Grass_2(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name){}
	~Grass_2(){}
};

//��
class Cabin : public StageObject
{
public:
	Cabin(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name)
	{
		//�����蔻��p���b�V��
		m_HitMesh.SetAsset("Cabin_Collision");
		m_HitMesh.SetTranselate(pos.x, pos.y, pos.z);
		m_HitMesh.SetRotationDegree((int)rot.x, (int)rot.y, (int)rot.z);
		m_HitMesh.SetScale(sca.x, sca.y, sca.z);

		//�L�����N�^�ƉƂ̓����蔻��
		m_CharacterHit.Regist_SMesh_vs_S(&m_HitMesh);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2);

		//�e�̓����蔻��
		m_BulletHit.Regist_SMesh_vs_L(&m_HitMesh, true);
		m_BulletHit.SetID(eHITID5, eHITID2);
		m_BulletHit.SetName("Cabin");

		//�J�����Ƃ̓����蔻��
		m_CameraHIt.Regist_SMesh_vs_L(&m_HitMesh, true);
		m_CameraHIt.SetID(eHITID3, eHITID2);
	}
	~Cabin(){}

private:
	StaticMesh m_HitMesh;
	Collider m_BulletHit;
	Collider m_CameraHIt;
};

//�n��
class Ground : public StageObject
{
public:
	Ground(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) 
	{
		m_CharacterHit.Regist_SMesh_vs_S(&m_Object);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2 | eHITID3);

		m_CameraHit.Regist_SMesh_vs_L(&m_Object, true);
		m_CameraHit.SetID(eHITID1, eHITID0);

		m_BulletHit.Regist_SMesh_vs_L(&m_Object, true);
		m_BulletHit.SetID(eHITID6, eHITID2);
		m_BulletHit.SetName("Ground");
	}
	~Ground() {}

private:
	Collider m_CameraHit;
	Collider m_BulletHit;
};

//��
class RockWall : public StageObject
{
public:
	RockWall(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) 
	{
		//�����蔻��p�̃��b�V��������
		//m_HitMesh.SetAsset("RockWall");
		m_HitMesh.SetAsset("Wall_Collision");
		m_HitMesh.SetTranselate(pos.x, pos.y, pos.z);
		m_HitMesh.SetRotationDegree((int)rot.x, (int)rot.y, (int)rot.z);
		m_HitMesh.SetScale(sca.x, sca.y, sca.z);

		//�L�����N�^�ƕǂ̓����蔻��
		m_CharacterHit.Regist_SMesh_vs_S(&m_HitMesh);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2 | eHITID3);

		//�J�����ƕǂ̓����蔻��
		m_CameraHit.Regist_SMesh_vs_L(&m_HitMesh, true);
		m_CameraHit.SetID(eHITID1, eHITID0);

		//�e�ƕǂ̓����蔻��
		m_BulletHit.Regist_SMesh_vs_L(&m_Object, true);
		m_BulletHit.SetID(eHITID7, eHITID2);
		m_BulletHit.SetName("RockWall");
	}
	~RockWall() {}

private:
	StaticMesh m_HitMesh;
	Collider m_CameraHit;
	Collider m_BulletHit;
};

//��
class SkyDome : public StageObject
{
public:
	SkyDome(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) {}
	~SkyDome() {}
};

//�΂�
class Bush : public StageObject
{
public:
	Bush(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) {}
	~Bush() {}
};

//�N���[�o�[
class Clover : public StageObject
{
public:
	Clover(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) {}
	~Clover() {}
};

//�V�_
class Fern : public StageObject
{
public:
	Fern(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) {}
	~Fern() {}
};

//�t�F���X
class Fence : public StageObject
{
public:
	Fence(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) {}
	~Fence() {}
};

//��A
class Stone_A : public StageObject
{
public:
	Stone_A(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) {}
	~Stone_A() {}
};

//�����t
class Fallen_leaves : public StageObject
{
public:
	Fallen_leaves(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) {}
	~Fallen_leaves() {}
};

//�͂��
class Tree_1_Trunk : public StageObject
{
public:
	Tree_1_Trunk(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) {}
	~Tree_1_Trunk() {}
};

//���
class Shrub : public StageObject
{
public:
	Shrub(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) {}
	~Shrub() {}
};

//���
class Weeds : public StageObject
{
public:
	Weeds(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name) {}
	~Weeds() {}
};

//�ؔ�
class WoodBox : public StageObject
{
public:
	WoodBox(XYZ pos, XYZ rot, XYZ sca, std::string name) :
		StageObject(pos, rot, sca, name)
	{
		//�v���C���[�̔���͉�
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

