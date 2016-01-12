#include "GEKO\\GEKO.h"
#include "GameSystem\StageObject.h"
#include "Scene\MainGame.h"

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	Debug::Start();
	Debug::SearchMemoryLeak();
	GEKO::Init("GEKO3DX", 800, 600);
	GEKO::MouseErasure();

	DirectionalLight::SetIntensity(0.6f);
	DirectionalLight::SetDirection(0.0f, 45.0f);

	new MainGame();

	while (GEKO::Loop())
	{
		GEKO::BackgroundColor(0, 0, 0);
		GEKO::DrawFps();
		StageObjectManager::GetInstance()->Render();
		TaskManager::Update();
		RenderManager::Render();
	}
	
	GEKO::End();
	Debug::End();

	return 0;
}