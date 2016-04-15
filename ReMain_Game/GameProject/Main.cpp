#include "GEKO\GEKO.h"
#include "GameSystem\StageObject\StageObject.h"
#include "Scene\Title.h"

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	GEKO::Init("Remain", 800, 600, 800, 600);
	GEKO::MouseErasure();
	GEKO::SetResolution_And_RefreshRate(1980, 1080, 60);
	GEKO::FullScreen(true);

	Camera::SetEye(0.0f, 0.0f, 0.0f);
	Camera::SetLookat(0.0f, 0.0f, 0.0f);
	Camera::FogDensity(0.05f);
	Camera::FogColor(240, 240, 240);

	DirectionalLight::SetIntensity(0.4f);
	DirectionalLight::SetDirection(0.0f, 30.0f);

	GEKO::ChangeFps(30);

	ImageAsset::LoadFile("TextData\\Image1.txt");
	StaticMeshAsset::LoadFile("TextData\\StaticMesh1.txt");
	SoundAsset::LoadFile("TextData\\Sound_Title.txt");

	new Title();

	while (GEKO::Loop())
	{
		GEKO::BackgroundColor(0, 0, 0);
		StageObjectManager::GetInstance()->Update();
	}

	GEKO::End();
	Debug::End();

	return 0;
}