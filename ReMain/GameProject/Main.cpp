#include "GEKO\\GEKO.h"
#include "Scene\Scene.h"
#include "MainGame\Player.h"

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	//Debug::Start();
	//Debug::SearchMemoryLeak();
	//GEKO::WindowFixing();
	GEKO::Init(L"GEKO3DX", 800, 600);

	DirectionalLight::SetIntensity(0.8f);
	DirectionalLight::SetDirection(0.0f, 0.5f);

	SceneManager::Quit(0, EScene::E_MAIN);

	while (GEKO::Loop())
	{
		GEKO::BackgroundColor(0, 0, 0);
		GEKO::DrawFps();
		SceneManager::Update();
		SceneManager::Render();
	}
	
	SceneManager::Clear();
	GEKO::End();
	//Debug::End();

	return 0;
}