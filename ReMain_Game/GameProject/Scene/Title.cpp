#include "Title.h"
#include "..\GEKO\GEKO.h"
#include "../GameSystem/StageObject/StageObject.h"
#include "MainGame.h"
#include <thread>

Image g_titleImage;
bool g_isLoad = false;

Title::Title() :
	Task("Title", 0)
{
	ImageAsset::LoadFile("TextData\\Image.txt");
	g_titleImage.SetAsset("SelectCircle");

	auto func = [] 
	{ 
		StaticMeshAsset::LoadFile("TextData\\StaticMesh.txt");
		DynamicMeshAsset::LoadFile("TextData\\DynamicMesh.txt"); 
		g_isLoad = true; 
	};

	auto func2 = []
	{
		/*
		float cnt = 0.0f;
		for (;;)
		{
			if (g_isLoad) break;
			GEKO::ScreenUpdate();
			GEKO::ClearColor(0, 255, 255);

			cnt += 0.5f;
			g_titleImage.SetAngle(cnt);
			WindowSize size = *Window::Get()->GetWindowSize();
			g_titleImage.Draw(size.sWidth / 2, size.sHeight / 2);
		}
		*/
		GEKO::ClearColor(0, 255, 255);
		WindowSize size = *Window::Get()->GetWindowSize();
		g_titleImage.Draw(size.sWidth / 3, size.sHeight / 3);
		GEKO::ScreenUpdate();
	};

	std::thread t1(func);
	std::thread t2(func2);

	t1.join();
	t2.join();

	new MainGame;

	Task::SetKill();
}

Title::~Title()
{
}

void Title::Update()
{
}

void Title::Render()
{
}