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

	//待機時間
	if ((m_WaitingCnt >= m_pWanderingsInfo->stopTime[m_TargetNum]) && m_IsStopTime)
	{
		m_pSearch->StartMove();
		m_IsStopTime = false;
		m_WaitingCnt = 0.0f;
	}
	else
	{
		//目標地点にきたら設定された時間待機と目標地点変更
		if (m_pSearch->GetIsGoal() && !m_IsStopTime)
		{
			//徘徊の回数が最大まできたら最初から
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
	str += "の読み込みに失敗しました";
	if (ifs.fail()) MessageBoxA(0, str.c_str(), NULL, MB_OK);

	while (!ifs.eof())
	{
		ifs >> str;

		if (str == "WanderingsInfo")
		{
			int cnt = 0;

			//名前読み込み
			ifs >> str;
			auto *p = &GetInstance()->m_pWanderingsPimpl->info[str];

			//向かう場所の数読み込み
			ifs >> str;
			p->targetNum = 0;
			p->targetNum = std::stoi(str);
			assert(p->targetNum > 0 && "ターゲット数がありません");

			//移動スピード読み込み
			ifs >> str;
			if (str == "Speed")
			{		
				for (cnt = 0; cnt < p->targetNum; cnt++)
				{
					str = "NULL";
					ifs >> str;
					assert(str != "NULL" && "WanderingのSpeedの値がありません");
					p->speed.emplace_back(std::stof(str));
				}
			}

			//待機時間
			ifs >> str;
			if (str == "StopTime")
			{
				for (cnt = 0; cnt < p->targetNum; cnt++)
				{
					str = "NULL";
					ifs >> str;
					assert(str != "NULL" && "WanderingのStopTimeの値がありません");
					p->stopTime.emplace_back(std::stof(str));
				}
			}

			//向かう場所
			ifs >> str;
			if (str == "TargetPos")
			{
				for (cnt = 0; cnt < p->targetNum; cnt++)
				{
					std::string pos[3] = {"NULL", "NULL", "NULL"};
					ifs >> pos[0];
					ifs >> pos[1];
					ifs >> pos[2];

					assert(pos[0] != "NULL" && "WanderingのTargetPosのX座標がありません");
					assert(pos[1] != "NULL" && "WanderingのTargetPosのY座標がありません");
					assert(pos[2] != "NULL" && "WanderingのTargetPosのZ座標がありません");

					p->targetPos.emplace_back(std::stof(pos[0]), std::stof(pos[1]), std::stof(pos[2]));
				}
			}

			//指定の数分読み込まれたか確認
			if (p->targetNum != p->speed.size()) assert(0 && "WanderingのSpeedの読み込みが適切ではありません");
			if (p->targetNum != p->stopTime.size()) assert(0 && "WanderingのStopTimeの読み込みが適切ではありません");
			if (p->targetNum != p->targetPos.size()) assert(0 && "WanderingのTargetPosの読み込みが適切ではありません");
		}
	}
}

WanderingsInfo *WanderingsManager::GetWanderingsInfo(std::string myName)
{
	return &GetInstance()->m_pWanderingsPimpl->info[myName];
}