#include "Wanderings.h"
#include <map>
#include <vector>

class WanderingsManagre;

struct WanderingsInfo
{
	std::vector<Vector3D> targetPos;
	std::vector<float> stopTime;
	std::vector<float> speed;
	int targetNum;
};

Wanderings::Wanderings() :
	m_TargetNum(0),
	m_WaitingCnt(0.0f),
	m_IsStart(true),
	m_IsStopTime(false)
{
}

Wanderings::~Wanderings()
{
}

bool Wanderings::GetIsStopTime()
{
	return m_IsStopTime;
}

void Wanderings::Init(std::string name, TracerouteSearch *pSearch,
					  Vector3D *pPos, Vector3D *pRot)
{
	m_pWanderingsInfo = WanderingsManager::GetWanderingsInfo(name);
	m_pSearch = pSearch;
	m_pPos = pPos;
	m_pRot = pRot;
	m_WaitingCnt = 0.0f;
	m_TargetNum = 0;
	m_IsStart = true;
	m_pSearch->StartMove();
	m_pSearch->StartSerch();
	m_pSearch->SetTargetName("Wanderings");

	m_Target.SetName("Wanderings");
	m_Target.SetPosition(&m_pWanderingsInfo->targetPos[m_TargetNum]);
}

void Wanderings::Update()
{
	if (!m_IsStart)
	{
		m_WaitingCnt = 0.0f;
		return;
	}

	if (m_IsStopTime)
	{
		m_WaitingCnt++;
	}

	//�ҋ@����
	if ((m_WaitingCnt >= m_pWanderingsInfo->stopTime[m_TargetNum]) && m_IsStopTime)
	{
		m_pSearch->StartMove();
		m_IsStopTime = false;
		m_WaitingCnt = 0.0f;
	}
	else
	{
		//�ڕW�n�_�ɂ�����ݒ肳�ꂽ���ԑҋ@�ƖڕW�n�_�ύX
		if (m_pSearch->GetIsGoal() && !m_IsStopTime)
		{
			//�p�j�̉񐔂��ő�܂ł�����ŏ�����
			if (m_TargetNum >= m_pWanderingsInfo->targetNum - 1)
			{
				m_TargetNum = 0;
			}
			else
			{
				m_TargetNum++;
			}
			m_Target.SetPosition(&m_pWanderingsInfo->targetPos[m_TargetNum]);
			m_pSearch->SetPosition(m_pPos);
			m_pSearch->StartSerch();
			m_pSearch->StopMove();
			m_IsStopTime = true;
		}
		else
		{
			m_pSearch->StopSerch();
			*m_pPos += *m_pSearch->GetTargetDirection() * m_pWanderingsInfo->speed[m_TargetNum] * GEKO::GetOneFps();
			m_pRot->y = atan2f(m_pSearch->GetTargetDirection()->x, m_pSearch->GetTargetDirection()->z);
		}
	}
}

void Wanderings::Start()
{
	if (!m_IsStart)
	{
		m_IsStart = true;
		m_TargetNum = 0;
		m_pSearch->StartSerch();
		m_Target.SetPosition(&m_pWanderingsInfo->targetPos[m_TargetNum]);
	}
}

void Wanderings::Stop()
{
	m_IsStart = false;
}

class WanderingsManager::WanderingsPimpl
{
public:
	std::map<std::string, WanderingsInfo> info;
};

WanderingsManager::WanderingsManager()
{
	m_pWanderingsPimpl = new WanderingsPimpl;
}

WanderingsManager::~WanderingsManager()
{
	for (auto& i : m_pWanderingsPimpl->info)
	{
		i.second.speed.clear();
		i.second.speed.shrink_to_fit();
		i.second.stopTime.clear();
		i.second.stopTime.shrink_to_fit();
		i.second.targetPos.clear();
		i.second.targetPos.shrink_to_fit();
	}

	delete m_pWanderingsPimpl;
}

WanderingsManager *WanderingsManager::GetInstance()
{
	static WanderingsManager wanderingsManager;
	return &wanderingsManager;
}

void WanderingsManager::LoadFile(const char* fileName)
{
	std::ifstream ifs(fileName);
	std::string str = fileName;
	str += "�̓ǂݍ��݂Ɏ��s���܂���";
	if (ifs.fail()) MessageBoxA(0, str.c_str(), NULL, MB_OK);

	while (!ifs.eof())
	{
		ifs >> str;

		if (str == "WanderingsInfo")
		{
			int cnt = 0;

			//���O�ǂݍ���
			ifs >> str;
			auto *p = &GetInstance()->m_pWanderingsPimpl->info[str];

			//�������ꏊ�̐��ǂݍ���
			ifs >> str;
			p->targetNum = 0;
			p->targetNum = std::stoi(str);
			assert(p->targetNum > 0 && "�^�[�Q�b�g��������܂���");

			//�ړ��X�s�[�h�ǂݍ���
			ifs >> str;
			if (str == "Speed")
			{		
				for (cnt = 0; cnt < p->targetNum; cnt++)
				{
					str = "NULL";
					ifs >> str;
					assert(str != "NULL" && "Wandering��Speed�̒l������܂���");
					p->speed.emplace_back(std::stof(str));
				}
			}

			//�ҋ@����
			ifs >> str;
			if (str == "StopTime")
			{
				for (cnt = 0; cnt < p->targetNum; cnt++)
				{
					str = "NULL";
					ifs >> str;
					assert(str != "NULL" && "Wandering��StopTime�̒l������܂���");
					p->stopTime.emplace_back(std::stof(str));
				}
			}

			//�������ꏊ
			ifs >> str;
			if (str == "TargetPos")
			{
				for (cnt = 0; cnt < p->targetNum; cnt++)
				{
					std::string pos[3] = {"NULL", "NULL", "NULL"};
					ifs >> pos[0];
					ifs >> pos[1];
					ifs >> pos[2];

					assert(pos[0] != "NULL" && "Wandering��TargetPos��X���W������܂���");
					assert(pos[1] != "NULL" && "Wandering��TargetPos��Y���W������܂���");
					assert(pos[2] != "NULL" && "Wandering��TargetPos��Z���W������܂���");

					p->targetPos.emplace_back(std::stof(pos[0]), std::stof(pos[1]), std::stof(pos[2]));
				}
			}

			//�w��̐����ǂݍ��܂ꂽ���m�F
			if (p->targetNum != p->speed.size()) assert(0 && "Wandering��Speed�̓ǂݍ��݂��K�؂ł͂���܂���");
			if (p->targetNum != p->stopTime.size()) assert(0 && "Wandering��StopTime�̓ǂݍ��݂��K�؂ł͂���܂���");
			if (p->targetNum != p->targetPos.size()) assert(0 && "Wandering��TargetPos�̓ǂݍ��݂��K�؂ł͂���܂���");
		}
	}
}

WanderingsInfo *WanderingsManager::GetWanderingsInfo(std::string myName)
{
	return &GetInstance()->m_pWanderingsPimpl->info[myName];
}