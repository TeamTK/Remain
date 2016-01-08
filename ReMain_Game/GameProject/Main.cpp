#include "GEKO\\GEKO.h"
#include "Scene\Scene.h"

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	Debug::Start();
	Debug::SearchMemoryLeak();
	GEKO::Init("GEKO3DX", 800, 600);
	GEKO::MouseErasure();

	DirectionalLight::SetIntensity(0.6f);
	DirectionalLight::SetDirection(0.0f, 45.0f);

	SceneManager::Quit(0, EScene::E_MAIN);

	while (GEKO::Loop())
	{
		GEKO::BackgroundColor(0, 0, 0);
		GEKO::DrawFps();
		SceneManager::Update();
		SceneManager::Render();
		TaskManager::Update();
		RenderManager::Render();
	}
	
	SceneManager::Clear();
	GEKO::End();
	Debug::End();

	return 0;
}