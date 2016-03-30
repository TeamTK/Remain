#ifndef _STAGE_OBJECT_
#define _STAGE_OBJECT_

#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include "../../GEKO/GEKO.h"

struct XYZ
{
	float x;
	float y;
	float z;

	XYZ() {};
	XYZ(float x, float y, float z) :
		x(x),
		y(y),
		z(z) {}
};

class MapObject;

class StageObject
{
public:
	StageObject();
	virtual ~StageObject();
	void Update();

protected:
	void SetObjectMesh(StaticMesh *pMesh, XYZ &pos, XYZ &rot, XYZ &sca, std::string &name)
	{
		pMesh->SetAsset(name);
		pMesh->SetTranselate(pos.x, pos.y, pos.z);
		pMesh->SetRotationDegree((int)rot.x, (int)rot.y, (int)rot.z);
		pMesh->SetScale(sca.x, sca.y, sca.z);
	}
};

class StageObjectManager
{
public:
	StageObjectManager();
	~StageObjectManager();
	void LoadObject(char* filename);
	void Update();
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
	void Update();
	void Relese();

private:
	StageObject *mp_StageObject;
};

//��1
class Tree_1 : public StageObject
{
public:
	Tree_1(XYZ pos, XYZ rot, XYZ sca, std::string name)
	{
		m_Object.SetRenderingRegister(true, 0, 0);
		m_Object.SetMeshState(eBlockingLight);
		SetObjectMesh(&m_Object, pos, rot, sca, name);

		m_Collision.SetAsset("Tree_1");
		m_Collision.SetTranselate(pos.x, pos.y, pos.z);
		m_Collision.SetRotationDegree((int)rot.x, (int)rot.y, (int)rot.z);
		m_Collision.SetScale(sca.x, sca.y, sca.z);

		m_CharacterHit.Regist_SMesh_vs_S(&m_Collision);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2);

		m_BulletHit.Regist_SMesh_vs_L(&m_Collision, true);
		m_BulletHit.SetID(eHITID4, eHITID2 | eHITID8);
		m_BulletHit.SetName("Tree_1");
	}
	~Tree_1() {}

private:
	StaticMesh m_Object;
	StaticMesh m_Collision;
	Collider m_CharacterHit;
	Collider m_BulletHit;
};

//��2
class Tree_2 : public StageObject
{
public:
	Tree_2(XYZ pos, XYZ rot, XYZ sca, std::string name)
	{
		m_Object.SetRenderingRegister(true, 0, 0);
		m_Object.SetMeshState(eBlockingLight);
		SetObjectMesh(&m_Object, pos, rot, sca, name);

		m_Collision.SetAsset("Tree_2");
		m_Collision.SetTranselate(pos.x, pos.y, pos.z);
		m_Collision.SetRotationDegree((int)rot.x, (int)rot.y, (int)rot.z);
		m_Collision.SetScale(sca.x, sca.y, sca.z);

		m_CharacterHit.Regist_SMesh_vs_S(&m_Collision);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2);

		m_BulletHit.Regist_SMesh_vs_L(&m_Collision, true);
		m_BulletHit.SetID(eHITID4, eHITID2 | eHITID8);
		m_BulletHit.SetName("Tree_2");
	}
	~Tree_2() {}

private:
	StaticMesh m_Object;
	StaticMesh m_Collision;
	Collider m_BulletHit;
	Collider m_CharacterHit;
};

//�͂��
class Tree_1_Trunk : public StageObject
{
public:
	Tree_1_Trunk(XYZ pos, XYZ rot, XYZ sca, std::string name)
	{
		m_Object.SetRenderingRegister(true, 0, 0);
		m_Object.SetMeshState(eBlockingLight);
		SetObjectMesh(&m_Object, pos, rot, sca, name);

		m_CharacterHit.Regist_SMesh_vs_S(&m_Object);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2);

		m_BulletHit.Regist_SMesh_vs_L(&m_Object, true);
		m_BulletHit.SetID(eHITID4, eHITID2);
		m_BulletHit.SetName("Tree_1_Trunk");
	}
	~Tree_1_Trunk() {}

private:
	StaticMesh m_Object;
	Collider m_CharacterHit;
	Collider m_BulletHit;
};

//��1
class Grass_1 : public StageObject
{
public:
	Grass_1(XYZ pos, XYZ rot, XYZ sca, std::string name)
	{
		m_Object.SetRenderingRegister(true, 0, 0);
		m_Object.SetMeshState(eBlockingLight);
		SetObjectMesh(&m_Object, pos, rot, sca, name);
	}
	~Grass_1() {}
private:
	StaticMesh m_Object;
};

//��2
class Grass_2 : public StageObject
{
public:
	Grass_2(XYZ pos, XYZ rot, XYZ sca, std::string name)
	{
		m_Object.SetRenderingRegister(true, 0, 0);
		m_Object.SetMeshState(eBlockingLight);
		SetObjectMesh(&m_Object, pos, rot, sca, name);
	}
	~Grass_2() {}
private:
	StaticMesh m_Object;
};

//��
class Cabin : public StageObject
{
public:
	Cabin(XYZ pos, XYZ rot, XYZ sca, std::string name)
	{
		m_Object.SetRenderingRegister(true, 0, 0);
		m_Object.SetMeshState(eShadow | eBlockingLight);
		SetObjectMesh(&m_Object, pos, rot, sca, name);

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
	~Cabin() {}

private:
	StaticMesh m_Object;
	StaticMesh m_HitMesh;
	Collider m_CharacterHit;
	Collider m_BulletHit;
	Collider m_CameraHIt;
};

//�n��_1_1
class Ground_1_1 : public StageObject
{
public:
	Ground_1_1(XYZ pos, XYZ rot, XYZ sca, std::string name)
	{
		m_Object.SetRenderingRegister(true, 0, 0);
		m_Object.SetMeshState(eShadow);
		SetObjectMesh(&m_Object, pos, rot, sca, name);

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
	StaticMesh m_Object;
	Collider m_CharacterHit;
	Collider m_CameraHit;
	Collider m_BulletHit;
};
//�n��_1_2
class Ground_1_2 : public StageObject
{
public:
	Ground_1_2(XYZ pos, XYZ rot, XYZ sca, std::string name)
	{
		m_Object.SetRenderingRegister(true, 0, 0);
		m_Object.SetMeshState(eShadow);
		SetObjectMesh(&m_Object, pos, rot, sca, name);

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
	StaticMesh m_Object;
	Collider m_CharacterHit;
	Collider m_CameraHit;
	Collider m_BulletHit;
};

//�n��_1_3
class Ground_1_3 : public StageObject
{
public:
	Ground_1_3(XYZ pos, XYZ rot, XYZ sca, std::string name)
	{
		m_Object.SetRenderingRegister(true, 0, 0);
		m_Object.SetMeshState(eShadow);
		SetObjectMesh(&m_Object, pos, rot, sca, name);

		m_CharacterHit.Regist_SMesh_vs_S(&m_Object);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2 | eHITID3);

		m_CameraHit.Regist_SMesh_vs_L(&m_Object, true);
		m_CameraHit.SetID(eHITID1, eHITID0);

		m_BulletHit.Regist_SMesh_vs_L(&m_Object, true);
		m_BulletHit.SetID(eHITID6, eHITID2 | eHITID8);
		m_BulletHit.SetName("Ground_1_3");
	}
	~Ground_1_3() {}

private:
	StaticMesh m_Object;
	Collider m_CharacterHit;
	Collider m_CameraHit;
	Collider m_BulletHit;
};

//��_1_1
class RockWall_1_1 : public StageObject
{
public:
	RockWall_1_1(XYZ pos, XYZ rot, XYZ sca, std::string name) 
	{
		m_Object.SetRenderingRegister(true, 0, 0);
		m_Object.SetMeshState(eShadow | eBlockingLight);
		SetObjectMesh(&m_Object, pos, rot, sca, name);

		//�����蔻��p�̃��b�V��������
		m_HitMesh.SetAsset("Wall_Collision_1_1");
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
		m_BulletHit.SetName("RockWall_1_1");
	}
	~RockWall_1_1() {}

private:
	StaticMesh m_Object;
	StaticMesh m_HitMesh;
	Collider m_CharacterHit;
	Collider m_CameraHit;
	Collider m_BulletHit;
};

//��_1_2
class RockWall_1_2 : public StageObject
{
public:
	RockWall_1_2(XYZ pos, XYZ rot, XYZ sca, std::string name)
	{
		m_Object.SetRenderingRegister(true, 0, 0);
		m_Object.SetMeshState(eShadow | eBlockingLight);
		SetObjectMesh(&m_Object, pos, rot, sca, name);

		//�����蔻��p�̃��b�V��������
		m_HitMesh.SetAsset("Wall_Collision_1_2");
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
		m_BulletHit.SetName("RockWall_1_2");
	}
	~RockWall_1_2() {}

private:
	StaticMesh m_Object;
	StaticMesh m_HitMesh;
	Collider m_CharacterHit;
	Collider m_CameraHit;
	Collider m_BulletHit;
};

//��_1_3
class RockWall_1_3 : public StageObject
{
public:
	RockWall_1_3(XYZ pos, XYZ rot, XYZ sca, std::string name)
	{
		m_Object.SetRenderingRegister(true, 0, 0);
		m_Object.SetMeshState(eShadow | eBlockingLight);
		SetObjectMesh(&m_Object, pos, rot, sca, name);

		//�����蔻��p�̃��b�V��������
		m_BulletHitMesh.SetAsset("RockWall_1_3");
		m_BulletHitMesh.SetTranselate(pos.x, pos.y, pos.z);
		m_BulletHitMesh.SetRotationDegree((int)rot.x, (int)rot.y, (int)rot.z);
		m_BulletHitMesh.SetScale(sca.x, sca.y, sca.z);

		m_HitMesh.SetAsset("Wall_Collision_1_3");
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
		m_BulletHit.Regist_SMesh_vs_L(&m_BulletHitMesh, true);
		m_BulletHit.SetID(eHITID7, eHITID2 | eHITID8);
		m_BulletHit.SetName("RockWall_1_3");
	}
	~RockWall_1_3() {}

private:
	StaticMesh m_Object;
	StaticMesh m_HitMesh;
	StaticMesh m_BulletHitMesh;
	Collider m_CharacterHit;
	Collider m_CameraHit;
	Collider m_BulletHit;
};

//��
class SkyDome : public StageObject
{
public:
	SkyDome(XYZ pos, XYZ rot, XYZ sca, std::string name)
	{
		m_Object.SetRenderingRegister(true, 0, 0);
		SetObjectMesh(&m_Object, pos, rot, sca, name);
	}
	~SkyDome() {}
private:
	StaticMesh m_Object;
};

//�΂�
class Bush : public StageObject
{
public:
	Bush(XYZ pos, XYZ rot, XYZ sca, std::string name)
	{
		m_Object.SetRenderingRegister(true, 0, 0);
		m_Object.SetMeshState(eShadow);
		SetObjectMesh(&m_Object, pos, rot, sca, name);
	}
	~Bush() {}
private:
	StaticMesh m_Object;
};

//�N���[�o�[
class Clover : public StageObject
{
public:
	Clover(XYZ pos, XYZ rot, XYZ sca, std::string name)
	{
		m_Object.SetRenderingRegister(true, 0, 0);
		m_Object.SetMeshState(eShadow);
		SetObjectMesh(&m_Object, pos, rot, sca, name);
	}
	~Clover() {}
private:
	StaticMesh m_Object;
};

//�V�_
class Fern : public StageObject
{
public:
	Fern(XYZ pos, XYZ rot, XYZ sca, std::string name) 
	{
		m_Object.SetRenderingRegister(true, 0, 0);
		m_Object.SetMeshState(eShadow | eBlockingLight);
		SetObjectMesh(&m_Object, pos, rot, sca, name);
	}
	~Fern() {}
private:
	StaticMesh m_Object;
};

//�t�F���X
class Fence : public StageObject
{
public:
	Fence(XYZ pos, XYZ rot, XYZ sca, std::string name)
	{
		m_Object.SetRenderingRegister(true, 0, 0);
		m_Object.SetMeshState(eShadow | eBlockingLight);
		SetObjectMesh(&m_Object, pos, rot, sca, name);

		m_HitMesh.SetAsset("Fence_Collision");
		m_HitMesh.SetTranselate(pos.x, pos.y, pos.z);
		m_HitMesh.SetRotationDegree((int)rot.x, (int)rot.y, (int)rot.z);
		m_HitMesh.SetScale(sca.x, sca.y, sca.z);

		//�L�����N�^�ƕǂ̓����蔻��
		m_CharacterHit.Regist_SMesh_vs_S(&m_HitMesh);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2);
	}
	~Fence() {}
private:
	StaticMesh m_Object;
	StaticMesh m_HitMesh;
	Collider m_CharacterHit;
};

//��A
class Stone_A : public StageObject
{
public:
	Stone_A(XYZ pos, XYZ rot, XYZ sca, std::string name)
	{
		m_Object.SetRenderingRegister(true, 0, 0);
		m_Object.SetMeshState(eShadow | eBlockingLight);
		SetObjectMesh(&m_Object, pos, rot, sca, name);

		m_HitMesh.SetAsset("Stone_A_Collision");
		m_HitMesh.SetTranselate(pos.x, pos.y, pos.z);
		m_HitMesh.SetRotationDegree((int)rot.x, (int)rot.y, (int)rot.z);
		m_HitMesh.SetScale(sca.x, sca.y, sca.z);

		//�L�����N�^�ƕǂ̓����蔻��
		m_CharacterHit.Regist_SMesh_vs_S(&m_HitMesh);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2 | eHITID3);

		//�J�����ƕǂ̓����蔻��
		m_CameraHit.Regist_SMesh_vs_L(&m_HitMesh, true);
		m_CameraHit.SetID(eHITID1, eHITID0);
	}
	~Stone_A() {}
private:
	StaticMesh m_Object;
	StaticMesh m_HitMesh;
	Collider m_CharacterHit;
	Collider m_CameraHit;
};

//�����t
class Fallen_leaves : public StageObject
{
public:
	Fallen_leaves(XYZ pos, XYZ rot, XYZ sca, std::string name)
	{
		m_Object.SetRenderingRegister(true, 0, 0);
		m_Object.SetMeshState(eShadow);
		SetObjectMesh(&m_Object, pos, rot, sca, name);
	}
	~Fallen_leaves() {}

private:
	StaticMesh m_Object;
};



//���
class Shrub : public StageObject
{
public:
	Shrub(XYZ pos, XYZ rot, XYZ sca, std::string name) 
	{
		m_Object.SetRenderingRegister(true, 0, 0);
		m_Object.SetMeshState(eBlockingLight);
		SetObjectMesh(&m_Object, pos, rot, sca, name);
	}
	~Shrub() 
	{

	}

private:
	StaticMesh m_Object;
};

//���
class Weeds : public StageObject
{
public:
	Weeds(XYZ pos, XYZ rot, XYZ sca, std::string name)
	{
		m_Object.SetRenderingRegister(true, 0, 0);
		m_Object.SetMeshState(eBlockingLight);
		SetObjectMesh(&m_Object, pos, rot, sca, name);
	}
	~Weeds() {}

private:
	StaticMesh m_Object;

};

//�ؔ�
class WoodBox : public StageObject
{
public:
	WoodBox(XYZ pos, XYZ rot, XYZ sca, std::string name)
	{
		m_Object.SetRenderingRegister(true, 0, 0);
		m_Object.SetMeshState(eShadow | eBlockingLight);
		SetObjectMesh(&m_Object, pos, rot, sca, name);

		//�v���C���[�̔���͉�
		m_CharacterHit.Regist_SMesh_vs_S(&m_Object);
		m_CharacterHit.SetID(eHITID0, eHITID1 | eHITID2);
		m_BulletHit.Regist_SMesh_vs_L(&m_Object, true);
		m_BulletHit.SetID(eHITID3, eHITID2);
		m_BulletHit.SetName("WoodBox");
	}
	~WoodBox() {}

private:
	StaticMesh m_Object;
	Collider m_CharacterHit;
	Collider m_BulletHit;
};
#endif

