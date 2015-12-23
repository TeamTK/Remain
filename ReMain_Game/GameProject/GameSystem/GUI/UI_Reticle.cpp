#include "UI_Reticle.h"


UI_Reticle::UI_Reticle() :
	Task("UI_Reticle", 1)
{
	m_RenderTask.Regist(1, REGIST_RENDER_FUNC(UI_Reticle::Draw));

	m_Reticle.SetAsset("Reticle");
	m_Reticle.SetCenter(32, 32);
	m_Reticle.SetAlpha(150);
}

UI_Reticle::~UI_Reticle()
{

}

void UI_Reticle::Update()
{
	printf("åƒÇÒÇ≈ÇÈÇ≈Å`\n");
}

void UI_Reticle::Draw()
{
	m_Reticle.Draw(400, 300);
}
