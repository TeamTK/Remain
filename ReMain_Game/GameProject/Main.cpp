#include "GEKO\GEKO.h"
#include "GameSystem\StageObject\StageObject.h"
#include "Scene\Title.h"
#include "MainGame\Enemy\Wanderings.h"

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	Debug::Start();
	Debug::SearchMemoryLeak();
	GEKO::Init("Remain", 800, 600, 800, 600);
	GEKO::MouseErasure();
	//GEKO::SetResolution_And_RefreshRate(1980, 1080, 60);
	//GEKO::FullScreen(true);

	Camera::SetEye(0.0f, 0.0f, 0.0f);
	Camera::SetLookat(0.0f, 0.0f, 0.0f);

	DirectionalLight::SetIntensity(0.4f);
	DirectionalLight::SetDirection(0.0f, 30.0f);

	GEKO::ChangeFps(30);

	ImageAsset::LoadFile("TextData\\Image.txt");
	StaticMeshAsset::LoadFile("TextData\\StaticMesh1.txt");

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