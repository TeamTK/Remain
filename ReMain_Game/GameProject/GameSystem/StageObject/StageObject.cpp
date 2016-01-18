#include "StageObject.h"
#include <sstream>

std::string className[] =
{
	"Tree_1",
	"Tree_2",
	"Grass_1",
	"Grass_2",
	"Cabin",
	"Ground",
	"RockWall",
	"SkyDome",
	"Bush",
	"Clover",
	"Fern",
	"Fence",
	"Stone_A",
	"Fallen_leaves",
	"Tree_1_Trunk",
	"Shrub",
	"Weeds"
};


StageObject::StageObject()
{

}

StageObject::StageObject(XYZ pos, XYZ rot, XYZ sca, std::string name)
{
	m_Object.SetAsset(name);
	m_Object.SetTranselate(pos.x, pos.y, pos.z);
	m_Object.SetRotationDegree((int)rot.x, (int)rot.y, (int)rot.z);
	m_Object.SetScale(sca.x, sca.y, sca.z);
}

StageObject::~StageObject()
{

}

void StageObject::Update()
{

}

void StageObject::Render()
{
	m_Object.SetAlphaAll(255);
	m_Object.Render();
}




//***************************
//	  StageObjectManager	*
//***************************

StageObjectManager* StageObjectManager::m_Instance = nullptr;

StageObjectManager::StageObjectManager() :
	m_cnt(0), m_number(0), m_num(0)
{

}

StageObjectManager::~StageObjectManager()
{

}

void StageObjectManager::LoadObject(char* filepath)
{
	std::ifstream ifs(filepath);
	std::string str;

	while (!ifs.eof())
	{
		ifs >> str;
		if (str == "number")
		{
			ifs >> str;
			m_number = std::stoi(str);
			m_num += m_number;
		}
		ifs >> str;
		if (str == "class")
		{
			ifs >> str;
			for (int i = 0; i < m_number; i++) {
				m_ClsName.emplace_back(str);
			}
		}
		ifs >> str;
		if (str == "name")
		{
			ifs >> str;
			for (int i = 0; i < m_number; i++) {
				m_MdlName.emplace_back(str);
			}
		}

		ifs >> str;
		if (str == "position")
		{
			for (int i = 0; i < m_number; i++)
			{
				ifs >> str;
				std::istringstream stream(str);
				getline(stream, str, ','); float x = std::stof(str);
				getline(stream, str, ','); float y = std::stof(str);
				getline(stream, str, ','); float z = std::stof(str);
				m_Position.emplace_back(x, y, z);
			}
		}
		ifs >> str;
		if (str == "rotation")
		{
			for (int i = 0; i < m_number; i++)
			{
				ifs >> str;
				std::istringstream stream(str);
				getline(stream, str, ','); float x = std::stof(str);
				getline(stream, str, ','); float y = std::stof(str);
				getline(stream, str, ','); float z = std::stof(str);
				m_Rotation.emplace_back(x, y, z);
			}
		}
		ifs >> str;
		if (str == "scale")
		{
			for (int i = 0; i < m_number; i++)
			{
				ifs >> str;
				std::istringstream stream(str);
				getline(stream, str, ','); float x = std::stof(str);
				getline(stream, str, ','); float y = std::stof(str);
				getline(stream, str, ','); float z = std::stof(str);
				m_Scale.emplace_back(x, y, z);
			}
		}
	}
	for (int i = 0; i < m_num; i++)
	{
		m_MapObjectList.emplace_back(m_Position[i], m_Rotation[i], m_Scale[i], m_ClsName[i]);
	}

	m_Position.shrink_to_fit();
	m_Rotation.shrink_to_fit();
	m_Scale.shrink_to_fit();
	m_ClsName.shrink_to_fit();
	m_MdlName.shrink_to_fit();

	m_Position.clear();
	m_Rotation.clear();
	m_Scale.clear();
	m_ClsName.clear();
	m_MdlName.clear();
}

void StageObjectManager::Update()
{

}

void StageObjectManager::Render()
{
	auto it = m_MapObjectList.begin();
	for (; it != m_MapObjectList.end();)
	{
		it->Render();
		it++;
	}
}

void StageObjectManager::ClearList()
{
	int a = m_MapObjectList.size();

	auto it = m_MapObjectList.begin();
	for (; it != m_MapObjectList.end();)
	{
		it->Relese();
		it = m_MapObjectList.erase(it);
	}
	m_MapObjectList.clear();
}

StageObjectManager* StageObjectManager::GetInstance()
{
	if (m_Instance == nullptr)
		m_Instance = new StageObjectManager();

	return m_Instance;
}

void StageObjectManager::ClearInstance()
{
	if (m_Instance != nullptr)
		delete (m_Instance);

	m_Instance = nullptr;
}

MapObject::MapObject()
{

}

MapObject::MapObject(XYZ &pos, XYZ &rot, XYZ &sca, std::string name)
{
	if (name == className[0])
	{
		mp_StageObject = new Tree_1(pos, rot, sca, name);
	}
	else if (name == className[1])
	{
		mp_StageObject = new Tree_2(pos, rot, sca, name);
	}
	else if (name == className[2])
	{
		mp_StageObject = new Grass_1(pos, rot, sca, name);
	}
	else if (name == className[3])
	{
		mp_StageObject = new Grass_2(pos, rot, sca, name);
	}
	else if (name == className[4])
	{
		mp_StageObject = new Cabin(pos, rot, sca, name);
	}
	else if (name == className[5])
	{
		mp_StageObject = new Ground(pos, rot, sca, name);
	}
	else if (name == className[6])
	{
		mp_StageObject = new RockWall(pos, rot, sca, name);
	}
	else if (name == className[7])
	{
		mp_StageObject = new SkyDome(pos, rot, sca, name);
	}
	else if (name == className[8])
	{
		mp_StageObject = new Bush(pos, rot, sca, name);
	}
	else if (name == className[9])
	{
		mp_StageObject = new Clover(pos, rot, sca, name);
	}
	else if (name == className[10])
	{
		mp_StageObject = new Fern(pos, rot, sca, name);
	}
	else if (name == className[11])
	{
		mp_StageObject = new Fence(pos, rot, sca, name);
	}
	else if (name == className[12])
	{
		mp_StageObject = new Stone_A(pos, rot, sca, name);
	}
	else if (name == className[13])
	{
		mp_StageObject = new Fallen_leaves(pos, rot, sca, name);
	}
	else if (name == className[14])
	{
		mp_StageObject = new Tree_1_Trunk(pos, rot, sca, name);
	}
	else if (name == className[15])
	{
		mp_StageObject = new Shrub(pos, rot, sca, name);
	}
	else if (name == className[16])
	{
		mp_StageObject = new Weeds(pos, rot, sca, name);
	}
	else
	{
		printf("StageObject‚Í¶¬‚³‚ê‚Ü‚¹‚ñ‚Å‚µ‚½ %s\n", name.c_str());
	}
}

MapObject::~MapObject()
{
}

void MapObject::Render()
{
	mp_StageObject->Render();
}

void MapObject::Relese()
{
	delete mp_StageObject;
}