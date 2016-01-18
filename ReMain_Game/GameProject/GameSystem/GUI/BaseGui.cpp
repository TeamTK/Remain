#include "BaseGui.h"

BaseGui::BaseGui() :
	Task("BaseGui", 0)
{
	m_RenderTask.Regist(0, REGIST_RENDER_FUNC(BaseGui::Draw));
}

BaseGui::~BaseGui()
{

}

void BaseGui::Update()
{

}

void BaseGui::Draw()
{
	m_Image.Draw((int)m_Pos.x, (int)m_Pos.y);
}
