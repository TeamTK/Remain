#include "RenderingManager.h"
#include "ConstantShader.h"
#include "DeferredShader\DeferredRendering.h"
#include "ForwardShader\ForwardRendering.h"
#include <map>
#include <list>

bool less(const RenderingInfo *data1, const RenderingInfo *data2)
{
	return data1->priority < data2->priority;
};

//Pimplイディオムで保持
class RenderingManager::ListPimpl
{
public:
	std::map<unsigned int, std::list<RenderingInfo*>> forwardRendering;
	std::map<unsigned int, std::list<RenderingInfo*>> deferredRendering;
	std::map<unsigned int, std::list<RenderingInfo*>> imageRendering;
};

RenderingManager::RenderingManager()
{
	m_pListPimpl = new ListPimpl;
}

RenderingManager::~RenderingManager()
{
	AllClear();
	delete m_pListPimpl;
}

RenderingManager *RenderingManager::GetInstance()
{
	static RenderingManager RenderingManager;
	return &RenderingManager;
}

void RenderingManager::AddForward(unsigned int priorityGroup, RenderingInfo *pRenderingInfo)
{
	m_pListPimpl->forwardRendering[priorityGroup].push_back(pRenderingInfo);
	SortForward(priorityGroup);
}

void RenderingManager::AddDeferred(unsigned int priorityGroup, RenderingInfo *pRenderingInfo)
{
	m_pListPimpl->deferredRendering[priorityGroup].push_back(pRenderingInfo);
	SortDeferred(priorityGroup);
}

void RenderingManager::AddImage(unsigned int priorityGroup, RenderingInfo *pRenderingInfo)
{
	m_pListPimpl->imageRendering[priorityGroup].push_back(pRenderingInfo);
	SortImage(priorityGroup);
}

void RenderingManager::Render()
{
	ID3D11DeviceContext *pDeviceContext = Direct3D11::GetInstance()->GetID3D11DeviceContext();

	ConstantShader::GetInstance()->SetCommonInfoConstantBuffer();

	/*
	//後方レンダリング
	DeferredRendering::GetInstance()->ChangeRenderTarget();
	for (auto& i : m_pListPimpl->deferredRendering)
	{
		for (auto& j : i.second)
		{
			j->func();
		}
	}
	ConstantShader::GetInstance()->SetDeferredLightConstantBuffer(pDeviceContext);
	DeferredRendering::GetInstance()->RenderingPass2();
	*/

	//前方レンダリング
	ConstantShader::GetInstance()->SetForwardLightConstantBuffer(pDeviceContext);
	for (auto& i : m_pListPimpl->forwardRendering)
	{
		for (auto& j : i.second)
		{
			j->func();
		}
	}

	//ForwardRendering::GetInstance()->Rendering();

	//画像を描画
	for (auto& i : m_pListPimpl->imageRendering)
	{
		for (auto& j : i.second)
		{
			j->func();
		}
	}
}

void RenderingManager::SortForward(unsigned int priorityGroup)
{
	m_pListPimpl->forwardRendering[priorityGroup].sort(&less);
}

void RenderingManager::SortDeferred(unsigned int priorityGroup)
{
	m_pListPimpl->deferredRendering[priorityGroup].sort(&less);
}

void RenderingManager::SortImage(unsigned int priorityGroup)
{
	m_pListPimpl->imageRendering[priorityGroup].sort(&less);
}

void RenderingManager::ClearForward(unsigned int priorityGroup, RenderingInfo *pRenderingInfo)
{
	auto it = m_pListPimpl->forwardRendering[priorityGroup].begin();
	auto itEnd = m_pListPimpl->forwardRendering[priorityGroup].end();
	for (; it != itEnd; it++)
	{
		if (*it == pRenderingInfo)
		{
			it = m_pListPimpl->forwardRendering[priorityGroup].erase(it);

			//リストの中がないならマップ部分消去
			unsigned int listNum = m_pListPimpl->forwardRendering[priorityGroup].size();
			if (listNum == 0) m_pListPimpl->forwardRendering.erase(priorityGroup);

			break;
		}
	}
}

void RenderingManager::ClearDeferred(unsigned int priorityGroup, RenderingInfo *pRenderingInfo)
{
	auto it = m_pListPimpl->deferredRendering[priorityGroup].begin();
	auto itEnd = m_pListPimpl->deferredRendering[priorityGroup].end();
	for (; it != itEnd; it++)
	{
		if (*it == pRenderingInfo)
		{
			it = m_pListPimpl->deferredRendering[priorityGroup].erase(it);

			//リストの中がないならマップ部分消去
			unsigned int listNum = m_pListPimpl->deferredRendering[priorityGroup].size();
			if (listNum == 0) m_pListPimpl->deferredRendering.erase(priorityGroup);

			break;
		}
	}
}

void RenderingManager::ClearImage(unsigned int priorityGroup, RenderingInfo *pRenderingInfo)
{
	auto it = m_pListPimpl->imageRendering[priorityGroup].begin();
	auto itEnd = m_pListPimpl->imageRendering[priorityGroup].end();
	for (; it != itEnd; it++)
	{
		if (*it == pRenderingInfo)
		{
			it = m_pListPimpl->imageRendering[priorityGroup].erase(it);

			//リストの中がないならマップ部分消去
			unsigned int listNum = m_pListPimpl->imageRendering[priorityGroup].size();
			if (listNum == 0) m_pListPimpl->imageRendering.erase(priorityGroup);

			break;
		}
	}
}

void RenderingManager::AllClear()
{
	for (auto& i : m_pListPimpl->forwardRendering)
	{
		i.second.clear();
	}
	m_pListPimpl->forwardRendering.clear();

	for (auto& i : m_pListPimpl->deferredRendering)
	{
		i.second.clear();
	}
	m_pListPimpl->deferredRendering.clear();

	for (auto& i : m_pListPimpl->imageRendering)
	{
		i.second.clear();
	}
	m_pListPimpl->imageRendering.clear();
}