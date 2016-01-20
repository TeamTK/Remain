#include "GEKO\GEKO.h"
#include "GameSystem\StageObject\StageObject.h"
#include "Scene\MainGame.h"
#include "Scene\Title.h"

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	Debug::Start();
	Debug::SearchMemoryLeak();
	GEKO::Init("Remain", 800, 600, 800, 600);
	GEKO::MouseErasure();

	DirectionalLight::SetIntensity(0.6f);
	DirectionalLight::SetDirection(0.0f, 45.0f);

	new Title();

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